#pragma once

#include "ui_mainwindow.h"

/** QT headers **/
#include <QVBoxLayout>
#include <QStringListModel>

/** JB headers **/
#include <jb/widgets/filelisteditor.h>
#include <jb/calendar/controls.h>

/** Application headers **/
#include "reporterpluginsmodel.h"
#include "pluginslistview.h"
#include "processorpluginsmodel.h"
#include "scheduleview.h"
#include "wrsync/schedulemodel.h"
#include "dailyschedulemodel.h"
#include "dailyscheduleview.h"
#include "filemapdialog.h"

using namespace JB;

class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = 0);

	private: // Methods
		void saveSettingsFromUi();
		void loadSettingsToUi();

		void setupUi();
		void setupModel();
		void connectUi();

	private: // Properties
		FileListEditor *sources_editor;
		ReporterPluginsModel *reportPlugins;

		PluginsListView *reportsListView;
		QVBoxLayout *reportConfigLayout,
					*processorConfigLayout;

		ProcessorPluginsModel *processorPlugins;
		ScheduleModel *schedule;
		DailyScheduleModel *daily_schedule;
		DailyScheduleView *dailyScheduleView;
		Calendar::Controls *calendar_controls;

		ScheduleView *schedule_view;
		QStringListModel *tab_names;
		FileMapDialog *file_map_dialog;

	private slots:
		void setReportPluginConfigUi(QWidget *config_ui);
		void setReportPluginConfigUi(QModelIndex index);

		void setProcessPluginConfigUi(int index) {
			setProcessPluginConfigUi(processorPlugins->index(index, 0));
		}

		void setProcessPluginConfigUi(QWidget *config_ui);
		void setProcessPluginConfigUi(QModelIndex index);

		void showAddFolderDialog();
		void loadDailySchedule(QDate date);
		void unschedule();

		void generateFileReport();

	protected:
		void changeEvent(QEvent *e);
		void closeEvent(QCloseEvent *event);
};

