#pragma once

/** QT headers **/
#include <QAbstractTableModel>
#include <QFontDatabase>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QFutureWatcher>

/** Application headers **/
#include "windowsfontregistry.h"

namespace FontMate {

	/**
		Fonts.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Fonts : public QAbstractTableModel {
		Q_PROPERTY(bool inlinePreview READ inlinePreview WRITE setInlinePreview)

		Q_OBJECT
		public:
			enum CollectionSource {None=0, Installed=1, Folder=2, PDF=3};
			enum FontIDType {FontID=0, SelectionID=1};
			enum FontDatabaseSource {QtFontSource=0, BuiltinFontSource=1};
			enum SelectionChangeNotification { NotifyChange, DontNotifyChange};

			/**
				Default constructor.
			*/
			Fonts(QObject *parent = 0);

			/**
				Destructor.
			*/
			~Fonts();

			/** QT Api **/
			int rowCount(const QModelIndex &parent=QModelIndex()) const;
			int columnCount(const QModelIndex &parent=QModelIndex()) const;
			QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
			Qt::ItemFlags flags(const QModelIndex &index) const;
			bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

			bool empty() const { return rowCount() == 0; }

			/**
				Get font family name

				@param id id (row) in this model.
				@return font family name.
			*/
			QString family(int id) const { return fonts[id]; }

			/**
				Load system fonts into model.
			*/
			void populateFrom(FontDatabaseSource font_database_source=QtFontSource);

			/**
				Load TrueType fonts from folder into model.
			*/
			void populateFrom(const QString path);

			/**
				Use the font for each line to render the font name

				@return True if use font to display font name.
			*/
			bool inlinePreview() const { return m_inline_preview; }

			void setInlinePreview(bool inline_preview) {
				m_inline_preview = inline_preview;
				reset();
			}

			/**
				Select font

				@param row_id ID of font to select.
			*/
			void selectFont(int row_id);

			void markFont(int row_id, SelectionChangeNotification notify=NotifyChange);
			void unmarkFont(int row_id);

			void unselectFont(int id, FontIDType id_type);

			void unselectAll();
			void unmarkAll();

			void changeFamily(QString old_family, QString new_family);

			bool familySelected(int id) { return selected_fonts.contains(id); }
			bool familyMarked(int id) { return marked_fonts.contains(id); }

			void unloadFonts();

			int previewFontSize() const { return preview_font_size; }

			CollectionSource collectionSource() const { return m_collection_source; }

			void setCollectionSource(CollectionSource source) {
				m_collection_source = source;
				emit fontCollectionSourceChanged();
			}

			bool selectionEmpty() const { return (selected_fonts.empty() and marked_fonts.empty()); }

			/**

				@param id ID (row) in model of selected font
				@return True if font is selected.
			*/
			bool selected(int id) const { return selected_fonts.contains(id); }

			bool installedFontsSelected() const;

			bool areLoading() {return m_loading; }

			void setLoading(bool loading) { m_loading = loading; }

			int selectedFontCount() const { return selected_fonts.count() + marked_fonts.count(); }

			/**
				Clear model data.
			*/
			void clear(bool reset_model=false);
			void clearSelection();

			QStringList families() const { return fonts; }

			QStringList selectedFontFamilies() const;

			QString previewText(QString family=QString()) const {
				if (m_preview_text.isEmpty())
					return family;
				return m_preview_text;
			}

			void copyFonts(QString family, QString destination) const;

		public slots:
			void setPreviewFontSize(int size) {
				preview_font_size = size;
				reset();
			}

			void installSelection();

			void uninstallSelection();

			void renameSelection();

			void cancelActions();

			void selectAll();

			void markAll();

			void setPreviewText(const QString &preview_text);

			void unselectFont(int id) { unselectFont(id, SelectionID); }

		signals:
			void fontSelected(const QString &font_family);
			void fontMarked();
			void fontRemoved(int id);
			void selectionChanged();
			void fontChanged(int id, QString font_family);

			void loadingFontsComplete(int font_count);
			void fontsToProcess(int count);
			void fontProcessed();

			void fontInstallStarted();
			void fontUninstallStarted();
			void fontLoadingStarted();

			void fontsInstalled();
			void fontsUninstalled();

			void fontCollectionSourceChanged();

		private:
			/** Properties **/

			QFontDatabase font_database;

			// The following 3 lists must be synced
			QStringList fonts; ///< Font families
			QMap<QString, QStringList> font_paths; ///< File the given font family was loaded from. (Full path)
			QMap<QString, int> loaded_font_ids; ///< Loaded font ids

			QList<int> selected_fonts; ///< Selected font ids. (ids are the rows in the model)
			QList<int> marked_fonts; ///< Fonts selected which are not in the preview viewer.

			QStringList available_font_paths;

			QFutureWatcher<int> *font_loader;
			QFutureWatcher<QString> *font_installer;
			QFutureWatcher<bool> *font_uninstaller;

			QFutureWatcher<void> *windows_font_installed;
			QFutureWatcher<void> *windows_font_uninstalled;

			QString m_path; ///< Path of current font collection

			CollectionSource m_collection_source;
			bool m_inline_preview;
			int preview_font_size;

			bool m_loading;

			QString m_preview_text;

			/** Methods **/

			int fontId(QString family) const;

			void emitRowChanged(int row);

			/**
				Add font files to family.
			*/
			void appendFontFiles(QString file_path, QString family);

			QStringList selectedFontFiles() const;


			/**
				@param id id of new font in qt font database.
				@return True if font was added to model.
			*/
			bool appendFontsById(int id);

			/**
				Checks if font file is installed in windows.
				First checks by full path, and if not found, checks by the file name (last element in path)

				@param font_family The files of this font family in the current data model will be checked if they are installed.
				@return True if font file installed
			*/
			bool fontInstalled(const QString &font_family) const;

		private slots:

			/**
				Reload current font source.
			*/
			void repopulate();

			void reloadWindowsFontRegistry() { WindowsFontRegistry::load(); }

			void processFontLoaderResult(int id);

			void forceReset() { reset(); }

			void notifyWindowsFontsInstalled();

			void notifyWindowsFontsUninstalled();

			/**
				Process result of installing font.
			*/
			void fontInstalled(int result_id);

			/**
				Process result of uninstalling font.
			*/
			void fontUninstalled(int result_id);

			void populateFromAvailableFiles();

			/**
				Notify the system that fonts were loaded into the model.
			*/
			void notifyFontsLoaded();

	};

}
