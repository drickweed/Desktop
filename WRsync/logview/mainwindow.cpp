#include "mainwindow.h"

/** QT headers **/
#include <QVBoxLayout>

/** JB headers **/
#include <jb/widgets/widget.h>

#include <wrsync/logfile.h>

using namespace JB;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent) {
	Ui::MainWindow::setupUi(this);
	setupModel();
	setupUi();
	connectUi();
}

void MainWindow::changeEvent(QEvent *e) {
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::setupUi() {
	calendar_controls = new Calendar::Controls();
	mainViewLayout->insertWidget(0, calendar_controls);
	calendar_controls->setDate(QDate::currentDate());

	calendar_view = new CalendarView(this);
	calendar_view->setModel(calendar_model);

	mainViewLayout->insertWidget(1, calendar_view);

	stateView->setModel(job_state);
	detailsView->hide();
	errorsView->hide();
	autoSize();
}

void MainWindow::autoSize() {
	resize(minimumSizeHint());
}

void MainWindow::connectUi() {
	connect(calendar_controls, SIGNAL(dateSelected(int, int)), calendar_model, SLOT(setDate(int, int)));
	connect(calendar_controls, SIGNAL(dateSelected(int, int)), calendar_model, SLOT(loadMonthResults(int, int)));
	connect(calendar_view, SIGNAL(dateSelected(QDate)), job_state, SLOT(loadDayResults(QDate)));
	connect(showDetailsButton, SIGNAL(clicked()), SLOT(autoSize()));
}

void MainWindow::setupModel() {
	calendar_model = new CalendarLogModel(this);
	errors_model= new CopyErrorsModel(this);
	job_state = new JobStateModel(this);

	calendar_model->setDate(QDate::currentDate());
	calendar_model->loadMonthResults();
}
