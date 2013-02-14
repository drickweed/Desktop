#pragma once

#include "ui_mainwindow.h"

/** JB headers **/
#include <jb/calendar/calendar_view.h>
#include <jb/calendar/calendar_model.h>
#include <jb/calendar/controls.h>

/** Application headers **/
#include "calendarlogmodel.h"
#include "copyerrorsmodel.h"
#include "jobstatemodel.h"

using namespace JB;

class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = 0);

	private:
		Calendar::Controls *calendar_controls;
		CalendarView *calendar_view;
		CalendarLogModel *calendar_model;
		JobStateModel *job_state;

		CopyErrorsModel *errors_model;

		void setupUi();
		void connectUi();
		void setupModel();

	private slots:
		void autoSize();

	protected:
		void changeEvent(QEvent *e);
};
