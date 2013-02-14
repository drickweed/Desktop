#include "fonts.h"

/** QT headers **/
#include <QFont>
#include <QFileInfo>
#include <QDir>
#include <QFontDatabase>
#include <QDebug>
#include <QFontMetrics>
#include <QSettings>
#include <QtConcurrentMap>
#include <QDesktopServices>
#include <QImage>
#include <QtConcurrentRun>

/** JB headers **/
#include <jb/qt4/pathutils.h>
#include <jb/qt4/exceptions.h>
#include <jb/qt4/fileutils.h>
#include <jb/std/log.h>

/** Application headers **/
#include <settings/settings.h>
#include <fonts/fontutils.h>
#include "windowsfontregistry.h"

using namespace JB;

namespace FontMate {

	Fonts::Fonts(QObject *parent):
		QAbstractTableModel(parent),
		m_collection_source(Fonts::None),
		m_inline_preview(false),
		preview_font_size(Settings().listFontSize()) {

		font_loader = new QFutureWatcher<int>(this);
		font_installer = new QFutureWatcher<QString>(this);
		font_uninstaller = new QFutureWatcher<bool>(this);
		windows_font_installed = new QFutureWatcher<void>(this);
		windows_font_uninstalled = new QFutureWatcher<void>(this);

		connect(windows_font_installed, SIGNAL(finished()), SIGNAL(fontsInstalled()));
		connect(windows_font_uninstalled, SIGNAL(finished()), SIGNAL(fontsUninstalled()));

		connect(font_loader, SIGNAL(started()), SIGNAL(fontLoadingStarted()));
		connect(font_loader, SIGNAL(resultReadyAt(int)), SLOT(processFontLoaderResult(int)));
		connect(font_loader, SIGNAL(resultReadyAt(int)), SIGNAL(fontProcessed()));
		connect(font_loader, SIGNAL(finished()), SLOT(notifyFontsLoaded()));

		//connect(font_installer, SIGNAL(resultReadyAt(int)), SLOT(fontInstalled(int)));
		connect(font_installer, SIGNAL(finished()), SLOT(notifyWindowsFontsInstalled()));
		connect(font_installer, SIGNAL(canceled()), SLOT(notifyWindowsFontsInstalled()));

		connect(this, SIGNAL(fontsInstalled()), SLOT(reloadWindowsFontRegistry()));
		connect(this, SIGNAL(fontsInstalled()), SLOT(forceReset()));

		//connect(font_uninstaller, SIGNAL(resultReadyAt(int)), SLOT(fontUninstalled(int)));
		connect(font_uninstaller, SIGNAL(finished()), SLOT(notifyWindowsFontsUninstalled()));
		connect(font_uninstaller, SIGNAL(canceled()), SLOT(notifyWindowsFontsUninstalled()));

		connect(this, SIGNAL(fontsUninstalled()), SLOT(forceReset()));
	}

	void Fonts::setPreviewText(const QString &preview_text) {
		m_preview_text = preview_text;
		emit dataChanged(index(0,0), index(rowCount()-1, 0));
	}

	Fonts::~Fonts() {
	}

	void Fonts::cancelActions() {
		if (font_installer->isRunning())
			font_installer->cancel();

		if (font_uninstaller->isRunning())
			font_uninstaller->cancel();

		if (font_loader->isRunning())
			font_loader->cancel();
	}

	void Fonts::notifyFontsLoaded() {
		emit loadingFontsComplete(rowCount());
		setLoading(false);
		reset();
	}

	void Fonts::clearSelection() {
		selected_fonts.clear();
		marked_fonts.clear();
	}

	void Fonts::repopulate() {
		reloadWindowsFontRegistry();

		switch (collectionSource()) {
			case Fonts::Installed:
				populateFrom(BuiltinFontSource);
				break;
			case Fonts::Folder:
				populateFrom(m_path);
				break;
			default:
				break;
		}
	}

	int Fonts::rowCount(const QModelIndex & /*parent*/) const {
		return fonts.count();
	}

	int Fonts::columnCount(const QModelIndex & /*parent*/) const {
		return 1;
	}

	void Fonts::copyFonts(QString family, QString destination) const {
		foreach(QString file, font_paths[family])
			File::copy(file, destination);
	}

	QVariant Fonts::data(const QModelIndex & index, int role) const {
		if (not index.isValid())
			return QVariant();

		int font_id = index.row();
		int column = index.column();
		QString font_family = fonts[font_id];

		switch (role) {
			case Qt::DecorationRole:
				if ((collectionSource() == Folder) and (fontInstalled(font_family)))
					return QVariant(QImage(":/tags/installed.png"));
				break;
			case Qt::DisplayRole:
				if (column == 0) {
					if (not inlinePreview())
						return QVariant(font_family);
					return QVariant(previewText(font_family));
				}
				break;
			case Qt::ToolTipRole:
				if (column == 0) {
					switch (collectionSource()) {
						case Fonts::Installed:
						case Fonts::Folder:
						case Fonts::PDF:
							return QVariant(font_paths[font_family].join("\n"));
						case Fonts::None:
							break;
					}
				}
				break;
			case Qt::FontRole:
				if ((column == 0) and (inlinePreview())) {
					LOG("Creating font object " << font_family.toStdString());
					return QVariant(QFont(font_family, previewFontSize()));
				}
				break;
			case Qt::CheckStateRole:
				if (column == 0) {
					if (selected_fonts.contains(font_id))
						return QVariant(Qt::Checked);
					if (marked_fonts.contains(font_id))
						return QVariant(Qt::PartiallyChecked);
					return QVariant(Qt::Unchecked);
				}
				break;
			case Qt::UserRole:
				return QVariant(font_family);
				break;
			default:
				break;
		}

		return QVariant();
	}

	void Fonts::populateFrom(FontDatabaseSource font_database_source) {
		clear();

		switch (font_database_source) {
			case QtFontSource:
				fonts = font_database.families();
				break;
			case BuiltinFontSource:
				reloadWindowsFontRegistry();
				available_font_paths = WindowsFontRegistry::registeredFontRealPaths();
				populateFromAvailableFiles();
				break;
		}

		setCollectionSource(Fonts::Installed);
		reset();
	}

	Qt::ItemFlags Fonts::flags(const QModelIndex & /*index*/) const {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;// | Qt::ItemIsTristate;
	}

	bool Fonts::setData(const QModelIndex & index, const QVariant & /* value */, int role) {
		if (not index.isValid())
			return false;

		bool edited = false;
		int column = index.column();
		int font_id = index.row();
		QVariant current_checked_state = data(index, role);

		switch(role) {
			case Qt::CheckStateRole:
				if (column == 0) {
					switch(current_checked_state.toInt()) {
						case Qt::Unchecked:
							selectFont(font_id);
							break;
						case Qt::Checked:
							unselectFont(font_id, FontID);
							break;
						case Qt::PartiallyChecked:
							unmarkFont(font_id);
							break;
						default:
							break;
					}
					edited = true;
				}
				break;
			default:
				break;
		}

		return edited;
	}

	void Fonts::selectFont(int row_id) {
		if (selected_fonts.contains(row_id))
			return;

		if (marked_fonts.contains(row_id))
			marked_fonts.removeOne(row_id);

		selected_fonts.append(row_id);
		emitRowChanged(row_id);
		emit fontSelected(fonts[row_id]);
		emit selectionChanged();
	}

	bool Fonts::installedFontsSelected() const {
		foreach(QString family, selectedFontFamilies())
			if (fontInstalled(family))
				return true;

		return false;
	}

	void Fonts::unselectFont(int id, FontIDType id_type) {
		if (id_type == SelectionID)
			id = selected_fonts[id];

		if (not selected_fonts.contains(id))
			return;

		int removed_font_index = selected_fonts.indexOf(id);
		selected_fonts.removeOne(id);
		emitRowChanged(id);
		emit fontRemoved(removed_font_index);
	}

	void Fonts::emitRowChanged(int row) {
		QModelIndex index = createIndex(row, 0);
		emit dataChanged(index, index);
	}

	int Fonts::fontId(QString /*family*/) const {
		return 0;
	}

	void Fonts::populateFrom(const QString path) {
		clear();
		setCollectionSource(Fonts::Folder);

		m_path = path;

		LOG("Populating model from path" << m_path.toStdString());

		QDir fonts_path(path);

		if (not fonts_path.exists())
			return;

		QStringList files = fonts_path.entryList(QDir::Files, QDir::Name | QDir::IgnoreCase);

		available_font_paths.clear();

		foreach(QString file, files)
			available_font_paths << Path::join(m_path, file);

		populateFromAvailableFiles();
	}

	void Fonts::populateFromAvailableFiles() {
		setLoading(true);
		font_loader->setFuture(QtConcurrent::mapped(available_font_paths, loadFont));
		emit fontsToProcess(available_font_paths.count());

	}

	void Fonts::clear(bool reset_model) {
		unloadFonts();
		setCollectionSource(Fonts::None);
		selected_fonts.clear();
		font_paths.clear();
		loaded_font_ids.clear();
		fonts.clear();
		m_path.clear();

		if (reset_model)
			reset();
	}

	bool Fonts::fontInstalled(const QString &font_family) const {
		foreach(QString font_path, font_paths[font_family])
			if (not ((fontInSystemPath(font_path)) and (WindowsFontRegistry::fontRegistered(font_path))))
				return false;
		return true;
	}

	void Fonts::unloadFonts() {
		loaded_font_ids.clear();
		QFontDatabase::removeAllApplicationFonts();
	}

	QStringList Fonts::selectedFontFiles() const {
		QStringList selected_font_paths;

		foreach(int id, selected_fonts)
			selected_font_paths << font_paths[fonts[id]];

		foreach(int id, marked_fonts)
			selected_font_paths << font_paths[fonts[id]];

		return selected_font_paths;
	}

	QStringList Fonts::selectedFontFamilies() const {
		QStringList selected_font_families;
		foreach(int id, selected_fonts)
			selected_font_families << fonts[id];
		foreach(int id, marked_fonts)
			selected_font_families << fonts[id];
		return selected_font_families;
	}

	bool Fonts::appendFontsById(int id) {
		if (id == -1)
			return false;

		QStringList new_fonts = QFontDatabase::applicationFontFamilies(id);

		if (fonts.indexOf(new_fonts[0]) == -1)
			fonts << new_fonts[0];

		loaded_font_ids[new_fonts[0]] = id;

		return true;
	}

	void Fonts::processFontLoaderResult(int id) {
		int font_id = font_loader->resultAt(id);

		if (font_id == -1) {
			// qDebug() << "Error loading font:" << available_font_paths[id];
			return;
		}

		if (appendFontsById(font_id))
			appendFontFiles(available_font_paths[id], fonts.last());
	}

	void Fonts::unselectAll() {
		while (not selected_fonts.empty())
			unselectFont(0);

		unmarkAll();
	}

	void Fonts::unmarkAll() {
		marked_fonts.clear();
		emit selectionChanged();
		reset();
	}

	void Fonts::appendFontFiles(QString file_path, QString family) {
		font_paths[family] << file_path;
	}

	void Fonts::fontInstalled(int result_id) {
		emitRowChanged(selected_fonts[result_id]);
	}

	void Fonts::fontUninstalled(int result_id) {
		emitRowChanged(selected_fonts[result_id]);
	}

	void Fonts::installSelection() {
		qDebug() << "Installing fonts:" << selectedFontFiles();
		font_installer->setFuture(QtConcurrent::mapped(selectedFontFiles(), FontMate::installFont));
		emit fontInstallStarted();
	}

	void Fonts::notifyWindowsFontsInstalled() {
		#ifdef Q_OS_WIN32
		windows_font_installed->setFuture(QtConcurrent::run(WindowsFontRegistry::notifyChanges));
		#endif
	}

	void Fonts::notifyWindowsFontsUninstalled() {
		#ifdef Q_OS_WIN32
		windows_font_uninstalled->setFuture(QtConcurrent::run(WindowsFontRegistry::notifyChanges));
		#endif
	}

	void Fonts::markFont(int row_id, SelectionChangeNotification notify) {
		if (selected_fonts.contains(row_id))
			return;

		if (marked_fonts.contains(row_id))
			return;

		marked_fonts.append(row_id);

		if (notify == NotifyChange) {
			emitRowChanged(row_id);
			emit fontMarked();
		}
	}

	void Fonts::unmarkFont(int row_id) {
		marked_fonts.removeOne(row_id);
		emitRowChanged(row_id);
		emit selectionChanged();
	}

	void Fonts::selectAll() {
		// This is so that in the preview all fonts will appear in the order they are in the list.
		unselectAll();

		for (int i=0; i<rowCount(); i++)
			selectFont(i);
	}

	void Fonts::markAll() {
		for (int i=0; i<rowCount(); i++)
			markFont(i, DontNotifyChange);

		emit selectionChanged();
		reset();
	}

	void Fonts::uninstallSelection() {
		QStringList fonts_to_uninstall;
		if (collectionSource() == Fonts::Folder) {
			foreach(QString font_path, selectedFontFiles())
				if (not installedFontPath(font_path).isEmpty())
					fonts_to_uninstall << installedFontPath(font_path);
		} else {
			fonts_to_uninstall = selectedFontFiles();
		}

		qDebug() << "Uninstalling font files:" << fonts_to_uninstall;
		font_uninstaller->setFuture(QtConcurrent::mapped(fonts_to_uninstall, FontMate::uninstallFont));
		emit fontUninstallStarted();
	}

	void Fonts::renameSelection() {
		if (collectionSource() == Fonts::Installed) {
			qDebug() << "Cannot rename installed fonts.";
			return;
		}

		foreach(QString font_path, selectedFontFiles()) {
			if (QFile(font_path).exists()) {
				LOG("Renaming font " << font_path.toStdString());
				renameFontWithFamilyName(font_path);
			}
		}

		// Font names were changed, model must be completely refreshed.
		unselectAll();
		repopulate();
	}

}
