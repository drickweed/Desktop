#pragma once

/** QT headers **/
#include <QMainWindow>
#include <QShortcut>
#include <QProgressBar>
#include <QSortFilterProxyModel>
#include <QTranslator>

/** Application headers **/
#include "ui_fontmate.h"
#include <document/document.h>
#include <widgets/fontpreviewstack.h>
#include <settings/settings.h>
#include <widgets/fontsview.h>
#include <widgets/tagmanager.h>
#include <widgets/tagselector.h>
#include <widgets/previewtypesview.h>
#include <widgets/fontpreviewsview.h>
#include <widgets/fontpreviewtabs.h>

/** JB headers **/
#include <jb/qt4/findfiles.h>
#include <jb/widgets/feedback.h>

namespace FontMate {

	const int MaximumStatusMessageLength = 100;

		/*
			Font-Mate main window.
		*/
		class MainWindow : public QMainWindow, public Ui::MainWindow {
		Q_OBJECT

		public:
			MainWindow(Document *document, QWidget* parent=0);

		private slots:
			void reloadTranslations();

			/** Install/Uninstall **/
			void installSelection();
			void uninstallSelection();
			void updateInstallButton();
			void enableInstallButton() {installSelectionButton->setEnabled(true); }
			void disableInstallButton() {installSelectionButton->setEnabled(false); }

			/** Settings **/
			void showSettingsDialog();
			void saveDefaultContent();
			void saveState();

			/** Tags/Labels **/
			void showTagManagerDialog();
			void showLabelsMenu();

			/** Preview settings **/
			void setInlineFontPreview(int state);
			void setBold(bool enabled);
			void setItalic(bool enabled);
			void setUnderline(bool enabled);
			void setFontSize(const QString &size) { setFontSize(QVariant(size).toInt()); }
			void setFontSize(int size);

			/** Font preview selection **/
			void clearFontSelection();
			void updateClearSelectionButton();
			void showSelectAllMenu();
			void updateSelectedFontsLabel();
			void loadFonts(int selection_index);
			void loadFonts(const QModelIndex &index);
			void advanceProgressBar();

			/** Donate **/
			/*
				Open donation window and mark in the settings that a donation has been made.
			*/
			void showDonationForm();
			void showDonationDialog();
			void showFeedbackDialog();

			/** Font folder management **/
			void addFolder();
			void removeSelectedFolder();
			void removeAllFontFolders();
			void removeFoldersWithoutFonts();

			/** Print **/
			void showPrintMenu();
			void printPreviews();
			void printFontListSelection();

			/** Copy **/
			void copySelectedFonts();

			/** Rename **/
			void renameSelectedFonts();

			/** Font folders **/
			void showFontFolderEditor(bool state);

			/** Font list **/
			void updateDisplayedFontCount(int font_count);

			/** Font search **/
			void setSearchResultCount(int count);

			/** Progress **/
			void updateProgressMessage(const QString &message);
			void hideProgressMessage();
			void showBusyProgress();
			void hideBusyProgress();
			void showFontLoadProgressBar(int count);

			void showRenameSelectedFontsButton();
			/** Editing **/
			void setDefaultFontSize(int);

		protected:
			virtual void closeEvent(QCloseEvent *event);

		private:
			/** Properties **/
			Document *document;
			Settings *settings;
			int current_selected_font_collection;
			FindFiles *find_files;
			FindFilesRunner *find_files_runner;
			QSortFilterProxyModel *sorted_font_folders;

			/** Shortcuts **/
			QShortcut *find_font_key, *find_tag_key;

			/** Widgets **/
			FontPreviewTabs *m_previews_tabs;

			QProgressBar *progressBar;
			FontsView *fontTableView;
			QPushButton *cancelButton;
			TagSelector *tagSelector;
			QLabel *progressMessage;
			PreviewTypesView *previewTypesView;

			FontPreviewsView *fontPreviews() const { return m_previews_tabs->m_previews; }

			/** Methods **/

			void setupUi();
			void setupModels();
			void loadSettings();
			void hideOptionalUi();
			void connectUi();

			void loadStandardFontSizes();

			/** Shortcuts **/
			Fonts* fonts() const { return document->fonts; }
			Folders* folders() const { return document->folders; }
			FolderSelectionModel* folderSelection() const { return document->folder_selection_model; }

			/**
				The index in the font folder combo box of the system installed folders.
			*/
			static int installedFontsIndex() { return 0; }

			/** Settings **/

			/**
				Load window locations and sizes.
			*/
			void loadState();

			/**
				Window location file.
			*/
			static QString stateFile() { return "fontmate.state"; }

			/**
				Save list of available font paths.
			*/
			void saveFontPaths();

			/**
				Load list of available font paths.
			*/
			void loadFontPaths();

			QTranslator *translator;

		protected:
			void changeEvent(QEvent *e);

	};

}
