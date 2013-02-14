#include "mainwindow.h"

/** QT headers **/
#include <QDebug>
#include <QFileDialog>
#include <QUrl>
#include <QMessageBox>
#include <QMenu>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextFrame>
#include <QTextCursor>
#include <QHeaderView>
#include <QTranslator>

/** Application headers **/
#include <settings/settings.h>
#include "about.h"
#include "config.h"
#include <fonts/fontutils.h>
#include <fonts/print.h>

#ifdef Q_OS_MAC
#include "menubar.h"
#endif

/** JB headers **/
#include <jb/std/log.h>
#include <jb/qt4/pathutils.h>

const int BUILTIN_FONT_FOLDER_COUNT=1;

namespace FontMate {

	MainWindow::MainWindow(Document *document, QWidget* parent):
		QMainWindow(parent),
		Ui::MainWindow(),
		document(document),
		current_selected_font_collection(0) {

		settings = new Settings(this);

		Ui::MainWindow::setupUi(this);
		setupModels();
		setupUi();
		hideOptionalUi();
		loadStandardFontSizes();
		connectUi();
		loadSettings();
		translator = new QTranslator(this);
		reloadTranslations();

#ifdef Q_OS_MAC
		osx::MenuBar::generate(this);
#endif

		if (not settings->donated())
			showDonationDialog();

		LOG("System font paths:");
		foreach(QString system_font_path, systemFontPaths) {
			LOG(system_font_path.toStdString());
		}
	}

	void MainWindow::reloadTranslations() {
		/** Language **/

		if (not translator->load(settings->translationFile())) {
			LOG("Failed to load translations");
		}
		qApp->installTranslator(translator);
	}

	void MainWindow::setupUi() {
		setWindowTitle(QString("%1 - %2").arg(FONTMATE_TITLE, FONTMATE_VERSION));

		fontTableView = new FontsView(this);
		fontTableView->setModel(document->font_search_proxy);

		qobject_cast<QVBoxLayout*>(fontListView->layout())->insertWidget(2, fontTableView);

		find_font_key = new QShortcut(QKeySequence("Ctrl+F"), this);
		//find_tag_key = new QShortcut(QKeySequence("Ctrl+T"), this);

		fontTableView->setFocus();

		m_previews_tabs = new FontPreviewTabs(this);

		instructions->setToolWidget(m_previews_tabs);

		updateInstallButton();
		updateClearSelectionButton();

		progressBar = new QProgressBar(this);
		cancelButton = new QPushButton(tr("Cancel"), this);
		progressMessage = new QLabel(this);

		Ui_MainWindow::statusBar->addPermanentWidget(progressMessage, 1);
		Ui_MainWindow::statusBar->addPermanentWidget(progressBar);
		Ui_MainWindow::statusBar->addPermanentWidget(cancelButton);

		progressBar->setMinimum(0);

		fontViewStack->setCurrentIndex(0);

		updateDisplayedFontCount(0);
		updateSelectedFontsLabel();

		foldersView->setModel(sorted_font_folders);
		foldersView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		foldersView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		foldersView->sortByColumn(1, Qt::AscendingOrder);

		fontCollection->setModel(document->folder_selection_model);
		fontCollection->setModelColumn(1);

		tagSelector = new TagSelector(document->tags, this);

#ifdef Q_OS_WIN
		// Do not show rename button on main font registry for windows
		renameFontsButton->hide();
#endif
	}

	void MainWindow::setupModels() {
		sorted_font_folders = new QSortFilterProxyModel(this);
		sorted_font_folders->setSourceModel(document->folders);

		find_files = new FindFiles(JB::Path::roots(), this);
		find_files->setFileGlobs(fontFileFilter);

		find_files_runner = new FindFilesRunner(find_files, this);
	}

	void MainWindow::hideOptionalUi() {
		listFontSize->hide();
		progressBar->hide();
		cancelButton->hide();
		searchOptionsButton->hide();
		labelsButton->hide();
		stopFontFolderSearchButton->hide();
		textEditingControls->hide();
		saveDefaultContentButton->hide();
		customPreviewTextLineEdit->hide();
		donateButton->setHidden(settings->donated());
	}

	void MainWindow::showFontFolderEditor(bool state) {
		if (state)
			fontViewStack->setCurrentIndex(1);
		else
			fontViewStack->setCurrentIndex(0);
	}

	void MainWindow::showFeedbackDialog() {
		Feedback feedback;
		feedback.setApplicationName("fontmate");
		feedback.exec();
	}

	void MainWindow::connectUi() {

		/** Previews **/
		connect(fontPreviews(), SIGNAL(previewContentChanged()), saveDefaultContentButton, SLOT(show()));
		connect(fontPreviews(), SIGNAL(fontPreviewAdded()), instructions, SLOT(showToolWidget()));
		connect(fontPreviews(), SIGNAL(fontPreviewAdded()), textEditingControls, SLOT(show()));
		connect(fontPreviews(), SIGNAL(previewRemoveRequested(int)), fonts(), SLOT(unselectFont(int)));

		/** Font folders **/
		connect(showFontFolderButton, SIGNAL(toggled(bool)), SLOT(showFontFolderEditor(bool)));

		/** Font list **/
		connect(inlinePreviewCheckbox, SIGNAL(stateChanged(int)), SLOT(setInlineFontPreview(int)));
		connect(showSelectedFontsCheckbox, SIGNAL(stateChanged(int)), document->font_search_proxy, SLOT(showOnlySelected(int)));
		connect(showSelectedFontsCheckbox, SIGNAL(stateChanged(int)), SLOT(updateSelectedFontsLabel()));
		connect(fonts(), SIGNAL(fontLoadingStarted()), SLOT(showRenameSelectedFontsButton()));

		/** Font search **/
		connect(fontSearch, SIGNAL(textEdited(const QString&)), document->font_search_proxy, SLOT(setFilterFixedString(const QString&)));
		connect(document->font_search_proxy, SIGNAL(searchResultsChanged(int)), SLOT(setSearchResultCount(int)));

		/** Font folders search **/
		connect(find_files_runner, SIGNAL(stopped()), stopFontFolderSearchButton, SLOT(hide()));
		connect(find_files_runner, SIGNAL(stopped()), startFontFolderSearchButton, SLOT(show()));
		connect(find_files_runner, SIGNAL(stopped()), SLOT(hideProgressMessage()));
		connect(find_files_runner, SIGNAL(stopped()), document->folders, SLOT(updateFontCounts()));
		connect(startFontFolderSearchButton, SIGNAL(clicked()), find_files_runner, SLOT(run()));
		connect(startFontFolderSearchButton, SIGNAL(clicked()),stopFontFolderSearchButton, SLOT(show()));
		connect(startFontFolderSearchButton, SIGNAL(clicked()),startFontFolderSearchButton, SLOT(hide()));

		connect(stopFontFolderSearchButton, SIGNAL(clicked()), find_files, SLOT(abort()));

		/** Font selection **/
		connect(fonts(), SIGNAL(fontSelected(const QString&)), fontPreviews(), SLOT(add(const QString&)));
		connect(fonts(), SIGNAL(selectionChanged()), SLOT(updateInstallButton()));
		connect(fonts(), SIGNAL(selectionChanged()), SLOT(updateClearSelectionButton()));
		connect(fonts(), SIGNAL(selectionChanged()), SLOT(updateSelectedFontsLabel()));
		connect(fonts(), SIGNAL(fontMarked()), SLOT(updateClearSelectionButton()));
		connect(fonts(), SIGNAL(fontCollectionSourceChanged()), SLOT(updateInstallButton()));
		connect(fonts(), SIGNAL(fontCollectionSourceChanged()), SLOT(updateClearSelectionButton()));

		/** Installation **/
		connect(uninstallSelectionButton, SIGNAL(clicked()), SLOT(uninstallSelection()));
		connect(installSelectionButton, SIGNAL(clicked()), SLOT(installSelection()));

		connect(fonts(), SIGNAL(fontsInstalled()), SLOT(enableInstallButton()));
		connect(fonts(), SIGNAL(fontsInstalled()), SLOT(hideBusyProgress()));
		connect(fonts(), SIGNAL(fontsInstalled()), SLOT(updateInstallButton()));

		connect(fonts(), SIGNAL(fontsUninstalled()), SLOT(enableInstallButton()));
		connect(fonts(), SIGNAL(fontsUninstalled()), SLOT(hideBusyProgress()));
		connect(fonts(), SIGNAL(fontsUninstalled()), SLOT(updateInstallButton()));

		connect(fonts(), SIGNAL(fontInstallStarted()), SLOT(disableInstallButton()));
		connect(fonts(), SIGNAL(fontUninstallStarted()), SLOT(disableInstallButton()));

		connect(document->folders, SIGNAL(rowsInserted(const QModelIndex& , int , int)), sorted_font_folders, SLOT(invalidate()));

		/** Font selection **/
		connect(fonts(), SIGNAL(fontRemoved(int)), fontPreviews(), SLOT(remove(int)));
		connect(fonts(), SIGNAL(fontRemoved(int)), SLOT(updateInstallButton()));
		connect(fonts(), SIGNAL(fontRemoved(int)), SLOT(updateClearSelectionButton()));
		connect(fonts(), SIGNAL(fontRemoved(int)), SLOT(updateSelectedFontsLabel()));

		connect(fonts(), SIGNAL(fontsToProcess(int)), SLOT(showFontLoadProgressBar(int)));
		connect(fonts(), SIGNAL(fontProcessed()), SLOT(advanceProgressBar()));
		connect(fonts(), SIGNAL(loadingFontsComplete(int)), SLOT(hideBusyProgress()));
		connect(fonts(), SIGNAL(loadingFontsComplete(int)), SLOT(updateDisplayedFontCount(int)));

		connect(fonts(), SIGNAL(fontInstallStarted()), SLOT(showBusyProgress()));
		connect(fonts(), SIGNAL(fontUninstallStarted()), SLOT(showBusyProgress()));
		connect(fonts(), SIGNAL(fontLoadingStarted()), SLOT(showBusyProgress()));

		/** About dialog **/
		connect(actionAbout, SIGNAL(triggered()), SLOT(showDonationDialog()));

		/** Shortcuts **/
		connect(find_font_key, SIGNAL(activated()), fontSearch, SLOT(setFocus()));

		connect(clearFontSelectionButton, SIGNAL(clicked()), SLOT(clearFontSelection()));
		connect(fonts(), SIGNAL(fontsUninstalled()), fontPreviews(), SLOT(clear()));

		/** Preview text editing **/
		connect(boldSetting, SIGNAL(clicked(bool)), SLOT(setBold(bool)));
		connect(italicSetting, SIGNAL(clicked(bool)), SLOT(setItalic(bool)));
		connect(underlineSetting, SIGNAL(clicked(bool)), SLOT(setUnderline(bool)));
		connect(fontSize, SIGNAL(currentIndexChanged(const QString&)), SLOT(setFontSize(const QString&)));
		connect(fontSize, SIGNAL(currentIndexChanged(const QString&)), fontPreviews(), SLOT(setDefaultFontSize(const QString&)));

		// FIXME: add explanation to this connection
		connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), fontPreviews(), SLOT(updateLastFocusedPreview(QWidget*, QWidget*)));

		connect(fontCollection, SIGNAL(activated(int)), SLOT(loadFonts(int)));
		connect(foldersView, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(loadFonts(const QModelIndex&)));

		connect(listFontSize, SIGNAL(valueChanged(int)), fonts(), SLOT(setPreviewFontSize(int)));
		connect(listFontSize, SIGNAL(valueChanged(int)), settings, SLOT(setListFontSize(int)));

		connect(fontTableView, SIGNAL(unselectedFontHighlighted(const QString&)), fontPreviews(), SLOT(loadInTemporaryView(const QString&)));

		connect(fontTableView, SIGNAL(selectedFontHighlighted(const QString&)), fontPreviews(), SLOT(removeTemporaryView()));

		/** Settings **/
		connect(saveDefaultContentButton, SIGNAL(clicked()), SLOT(saveDefaultContent()));
		connect(saveDefaultContentButton, SIGNAL(clicked()), saveDefaultContentButton, SLOT(hide()));

		connect(fontPreviews(), SIGNAL(lastFontPreviewRemoved()), saveDefaultContentButton, SLOT(hide()));
		connect(fontPreviews(), SIGNAL(lastFontPreviewRemoved()), instructions, SLOT(showInstructions()));
		connect(fontPreviews(), SIGNAL(lastFontPreviewRemoved()), textEditingControls, SLOT(hide()));
		connect(fontPreviews(), SIGNAL(temporaryPreviewRemoved()), fontTableView, SLOT(clearSelection()));

		connect(donateButton, SIGNAL(clicked()), SLOT(showDonationForm()));

		connect(actionSettings, SIGNAL(triggered()), SLOT(showSettingsDialog()));

		connect(cancelButton, SIGNAL(clicked()), fonts(), SLOT(cancelActions()));

		connect(addFolderButton, SIGNAL(clicked()), SLOT(addFolder()));
		connect(removeFolderButton, SIGNAL(clicked()), SLOT(removeSelectedFolder()));
		connect(removeAllFoldersButton, SIGNAL(clicked()), SLOT(removeAllFontFolders()));
		connect(removeFoldersWithoutFontsButton, SIGNAL(clicked()), SLOT(removeFoldersWithoutFonts()));

		connect(selectAllFontsButton, SIGNAL(clicked()), SLOT(showSelectAllMenu()));
		connect(selectWithPreviewAction, SIGNAL(triggered()), fonts(), SLOT(selectAll()));
		connect(selectWithoutPreviewAction, SIGNAL(triggered()), fonts(), SLOT(markAll()));

		connect(customPreviewTextLineEdit, SIGNAL(textEdited(const QString&)), fonts(), SLOT(setPreviewText(const QString&)));

		connect(printButton, SIGNAL(clicked()), SLOT(showPrintMenu()));

		connect(copyFontsButton, SIGNAL(clicked()), SLOT(copySelectedFonts()));

		connect(renameFontsButton, SIGNAL(clicked()), SLOT(renameSelectedFonts()));

		connect(labelsButton, SIGNAL(clicked()), SLOT(showLabelsMenu()));

		/** Actions **/
		connect(printPreviewsAction, SIGNAL(triggered()), SLOT(printPreviews()));
		connect(printListSelectionAction, SIGNAL(triggered()), SLOT(printFontListSelection()));

		connect(find_files, SIGNAL(pathsFound(QStringList)), document->folders, SLOT(addFolders(QStringList)));
		connect(find_files, SIGNAL(searchingPath(const QString&)), SLOT(updateProgressMessage(const QString&)));

		connect(settings, SIGNAL(donated(bool)), donateButton, SLOT(setHidden(bool)));

		// Update style button when text is selected or cursor moves around in any of the preview areas.
		connect(fontPreviews(), SIGNAL(selectionBold(bool)), boldSetting, SLOT(setChecked(bool)));
		connect(fontPreviews(), SIGNAL(selectionItalic(bool)), italicSetting, SLOT(setChecked(bool)));
		connect(fontPreviews(), SIGNAL(selectionUnderlined(bool)), underlineSetting, SLOT(setChecked(bool)));
		connect(fontPreviews(), SIGNAL(selectionSize(const QString&)), fontSize, SLOT(setEditText(const QString&)));

		connect(fontPreviews(), SIGNAL(formattingChanged()), saveDefaultContentButton, SLOT(show()));

		connect(feedbackButton, SIGNAL(clicked()), SLOT(showFeedbackDialog()));

		connect(settings, SIGNAL(languageChanged(QString)), SLOT(reloadTranslations()));
	}

	void MainWindow::printPreviews() {
		if (fonts()->selectionEmpty()) {
			QMessageBox::information(this, tr("No fonts selected."), tr("No fonts selected."));
			return;
		}

		QPrinter *printer = new QPrinter;
		QPrintDialog printDialog(printer, this);
		QTextDocument formatted_font_previews;
		QTextCursor cursor = formatted_font_previews.rootFrame()->firstCursorPosition();

		if (printDialog.exec() == QDialog::Accepted) {
			foreach(FontPreviewStack *preview, fontPreviews()->previews()) {
				// Print font family name
				cursor.insertBlock();
				cursor.insertHtml(QString("<h2 style=\"font-family:Arial; font-weight:normal; text-decoration:underline;\">%1</h2>").arg(preview->family()));

				// Print text preview
				cursor.insertBlock();
				cursor.insertHtml("<p>");
				cursor.insertHtml(preview->document()->toHtml());
				cursor.insertHtml("</p>");
			}
			formatted_font_previews.print(printer);
		}
		delete printer;
	}

	void MainWindow::printFontListSelection() {
		if (fonts()->selectionEmpty()) {
			QMessageBox::information(this, tr("No fonts selected."), tr("No fonts selected."));
			return;
		}
		Print::fontList(fonts()->selectedFontFamilies(), this, fonts()->previewText(), fonts()->previewFontSize());
	}

	void MainWindow::copySelectedFonts() {
		if (fonts()->selectionEmpty()) {
			QMessageBox::information(this, tr("No fonts selected."), tr("No fonts selected."));
			return;
		}

		QString path = QFileDialog::getExistingDirectory(this, tr("Select folder to copy fonts to"));
		if (path.isEmpty())
			return;

		foreach(QString family, fonts()->selectedFontFamilies())
			fonts()->copyFonts(family, path);
	}

	void MainWindow::updateDisplayedFontCount(int font_count) {
		if (font_count > 1)
			selectAllFontsButton->setText(QString("%1 %2").arg(tr("Select all")).arg(font_count));
		else
			selectAllFontsButton->setText(tr("Select all"));
		selectAllFontsButton->setVisible(font_count > 1);
	}

	void  MainWindow::showBusyProgress() {
		progressBar->setMaximum(0);
		progressBar->show();
		cancelButton->show();
	}

	void  MainWindow::hideBusyProgress() {
		progressBar->hide();
		cancelButton->hide();
	}

	void MainWindow::setInlineFontPreview(int state) {
		fonts()->setInlinePreview(state != Qt::Unchecked);
	}

	void MainWindow::removeSelectedFolder() {
		QModelIndexList selection = foldersView->selectionModel()->selectedRows();

		if (selection.empty())
			return;

		int selected_index = sorted_font_folders->mapToSource(selection[0]).row();

		// Removing the currently selected font folder. Clear the font list before removing
		// folder from the list.
		if (fontCollection->currentIndex() - BUILTIN_FONT_FOLDER_COUNT == selected_index)
			fonts()->clear(true);

		folders()->removeFolder(selected_index);
	}

	void MainWindow::removeAllFontFolders() {
		QMessageBox::StandardButton result = QMessageBox::question(this, tr("Remove all folders ?"), tr("Remove all folders ?"), QMessageBox::Yes | QMessageBox::No);

		if (result == QMessageBox::No)
			return;

		fontCollection->setCurrentIndex(0); // 0 Is always a system font folder
		folders()->clear();
	}

	void MainWindow::removeFoldersWithoutFonts() {
		QMessageBox::StandardButton result = QMessageBox::question(this, tr("Remove folders not containing fonts ?"), tr("Remove folders not containing fonts ?"), QMessageBox::Yes | QMessageBox::No);

		if (result == QMessageBox::No)
			return;

		fontCollection->setCurrentIndex(0); // 0 Is always a system font folder

		folders()->removeFoldersWithoutFonts();

	}

	void MainWindow::showPrintMenu() {
		QMenu menu(this);
		menu.addAction(printPreviewsAction);
		menu.addAction(printListSelectionAction);

		menu.exec(printButton->mapToGlobal(QPoint(0,0)));
	}

	void MainWindow::showSelectAllMenu() {
		QMenu menu(this);
		menu.addAction(selectWithoutPreviewAction);
		menu.addAction(selectWithPreviewAction);

		menu.exec(selectAllFontsButton->mapToGlobal(QPoint(0,0)));
	}

	void MainWindow::showLabelsMenu() {
		tagSelector->move(labelsButton->mapToGlobal(QPoint(0,0)));
		tagSelector->show();
	}

	void MainWindow::loadStandardFontSizes() {
		QList<int> sizes = QFontDatabase::standardSizes();
		QStringList sizes_str;
		foreach(int size, sizes)
			sizes_str << QVariant(size).toString();
		fontSize->addItems(sizes_str);
	}

	void MainWindow::loadFonts(const QModelIndex &index) {
		int folder_index_to_load = sorted_font_folders->mapToSource(index).row() + 1;
		fontCollection->setCurrentIndex(folder_index_to_load);
		loadFonts(folder_index_to_load);
		showFontFolderEditor(false);
		showFontFolderButton->setChecked(false);
	}

	void MainWindow::loadFonts(int selection_index) {
		if (selection_index == current_selected_font_collection)
			return;

		if ((selection_index < 0) or (selection_index >= fontCollection->count()))
			return;

		QString path;

		// Load system font list
		if (selection_index == installedFontsIndex()) {
			fontPreviews()->clear();
			fonts()->populateFrom(Fonts::BuiltinFontSource);
			current_selected_font_collection = selection_index;
			return;
		}

		// Let the user select a font folder
		if (folderSelection()->loadFolderItem(selection_index)) {
			path = QFileDialog::getExistingDirectory(this, tr("Select folder with fonts to load"));

			if (path.isEmpty() or not QDir(path).exists()) {
				fontCollection->setCurrentIndex(current_selected_font_collection);
				return;
			}

			int path_index = document->folder_selection_model->indexOfPath(path);
			if (path_index != -1) { // Path is already in the list.
				fontCollection->setCurrentIndex(path_index);
			} else {
				folders()->insertFolder(path);
				fontCollection->setCurrentIndex(1); // Path inserted at the top.
				saveFontPaths();
			}

		// Let the user select a PDF to load fonts from
		} else if (folderSelection()->loadPdfItem(selection_index)) {

		} else { // User selected folder from the list
			path = fontCollection->itemText(selection_index);
		}

		fontPreviews()->clear();
		fonts()->populateFrom(path);
		current_selected_font_collection = selection_index;
	}

	void MainWindow::showFontLoadProgressBar(int count) {
		progressBar->show();
		progressBar->setValue(0);
		progressBar->setMaximum(count);
	}

	void MainWindow::advanceProgressBar() {
		progressBar->setValue(progressBar->value() + 1);
	}

	void MainWindow::showDonationDialog() {
		About::Result result = (About::Result)About().exec();
		switch (result) {
			case About::AlreadyDonated:
				settings->setDonated(true);
				break;
			case About::Donate:
				showDonationForm();
				break;
			case About::DonateLater:
				break;
		}
		donateButton->setHidden(settings->donated());
	}

	void MainWindow::showDonationForm() {
		settings->setDonated(true);
		QDesktopServices::openUrl(QUrl("http://www.jbox-comp.com/fontmate/"));
	}

	void MainWindow::showTagManagerDialog() {
		TagManager(document, this).exec();
	}

	void MainWindow::closeEvent(QCloseEvent *event) {
		saveFontPaths();
		saveState();
		settings->sync();
		find_files->abort();
		event->accept();
	}

	void MainWindow::renameSelectedFonts() {
		QMessageBox::StandardButton result = QMessageBox::question(this, tr("Rename selected fonts ?"), tr("Rename selected fonts ?"), QMessageBox::Yes | QMessageBox::No);
		if (result == QMessageBox::Yes)
			fonts()->renameSelection();
	}

	void MainWindow::updateProgressMessage(const QString &message) {
		QString displayed_message = message;

		if (message.length() > MaximumStatusMessageLength)
			displayed_message = QString("...%1").arg(message.right(MaximumStatusMessageLength));
		progressMessage->setText(displayed_message);
	}

	void MainWindow::hideProgressMessage() {
		progressMessage->setText("");
	}

	void MainWindow::showRenameSelectedFontsButton() {
		renameFontsButton->setVisible(fonts()->collectionSource() != Fonts::Installed);
	}
}
