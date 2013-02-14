#pragma once

/** QT headers **/
#include <QMainWindow>
#include <QProgressBar>
#include <QCheckBox>
#include <QLabel>
#include <QShortcut>
#include <QSortFilterProxyModel>

/** JB headers **/
#include <jb/widgets/logwatcher.h>
#include <jb/widgets/fancylineedit.h>

#include "ui_auto-mate.h"

#include "installer.h"
#include "appsmodel.h"
#include "configurationui.h"
#include "tweaksmodel.h"

using namespace JB;

/**
	Main application GUI.
*/
class AutoMateUi : public QMainWindow, private Ui::AutoMate {
	Q_OBJECT

	private:
		/** Controllers **/
		TextFileWatcher *logWatcher; /// Log file watcher.
		ApplicationsModel *applications_model;
		TweaksModel *tweaks_model;

		/** Gui elements **/
		FancyLineEdit *searchEdit;
		QShortcut *searchShortcut;
		QProgressBar *downloadProgressBar;
		QCheckBox *download_checkBox;
		QAction *download_Action;
		QLabel *progressName;
		ConfigurationUi *config;
		QSortFilterProxyModel *tweaksProxyModel;
		QSortFilterProxyModel *appsProxyModel;

		bool m_controller_busy; /// Last busy state notified by controller (busy - true / false)

		int selectedApplicationCount;

		QString selectedTweakFileName; /// Filename of tweak selected by context menu

		std::string last_database_location; /// FIXME This is a hack to reload database on location change.

		/**
			Connect actions.
		*/
		void actionsConnect();

		/**
			Setup UI.
		*/
		void setupUi(QMainWindow *widget);

		/**
			Connect UI.
		*/
		void connectUi();

		bool busy() const { return m_controller_busy; }

	public slots:

		void setBusy() { m_controller_busy = true; }
		void setIdle() { m_controller_busy = false; }
		void updateDataReadProgress(int bytesRead, int totalBytes);
		void resetStartAction();
		void applyActionToStartButton();
		void updateStatusMessage(QString message);
		void updateProgressName(QString name);
		void applyAbortToStartButton();
		void updateDownloadSpeed(int kbps);
		void startedNewDownload();

	public:
		/**
			Default constructor.
			@param parent Parent for Qt heirarchy and memory management.
		*/
		AutoMateUi(QWidget* parent = 0);

		void setDownloadOnly(bool state) {
			download_checkBox->setChecked(state);
		}

		void loadTemplate(std::string filename);
		void applySelected();

	protected slots:

		void addToSelection(void *item);
		void removeFromSelection(void *item);

		void editSelectedTweak();

		/**
			Exits the application.
		*/
		void quit();

		/**
			Pops up "About" dialog.
		*/
		void about();

		/**
			Pops Export.. dialog and saves to hard-drive.
		*/
		void save();

		/**
			Pops Import.. dialog and updates selection.
		*/
		void load();

		/**
			Clears selection on applications view.
		*/
		void clearSelection();

		/**
			Applies the selected tweaks from the tweaks view.
		*/
		void tweak();

		/**
			Install selected applications (optionally downlod)
		*/
		void install();

		/**
			Filters the views by searchEdit's value.
		*/
		void filterViews();

		/**
			Forces reload of models.
		*/
		void reloadModels();

		/**
			Reload models if database location changed.

			FIXME ugly hack.
		*/
		void reloadModelsOnDatabaseChange();

		void hideHelp();

		void showContents();

		void showQuickHelp();

		void tweakContextMenu(const QPoint&);

#ifdef Q_OS_WIN32
		void requestReboot();
		void setUACState(bool state);
#endif

	signals:
		void install(std::vector<Version> versions, bool download_only);
		void tweak(std::vector<TweakAction> actions);
		void abort();
		void start();
};
