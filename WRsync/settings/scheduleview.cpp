#include "scheduleview.h"

/** QT headers **/
#include <QMenu>

/** Application headers **/
#include "wrsync/logfile.h"

ScheduleView::ScheduleView(QWidget *parent) :
	CalendarView(parent) {
	setContextMenuPolicy (Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(contextMenu(const QPoint&)));
}


void ScheduleView::contextMenu(const QPoint & point) {

	QModelIndex index = indexAt(point);
	if (!index.isValid())
		return;

	QMenu menu;
	QAction *action;

	// Find selected dates
	QItemSelectionModel *selection_model = selectionModel();
	QModelIndexList selection = selection_model->selectedIndexes();

	// Single date selected
	if (selection.size() == 1) {
		selected_date = dynamic_cast<CalendarModel*>(model())->date(selection[0]);

		action = menu.addAction(tr("Weekly"));
		connect(action, SIGNAL(triggered()), SLOT(scheduleWeeklyBackup()));

		action = menu.addAction(tr("Once"));
		connect(action, SIGNAL(triggered()), SLOT(scheduleOneBackup()));

		action = menu.addAction(tr("Daily"));
		connect(action, SIGNAL(triggered()), SLOT(scheduleDailyBackup()));

	} else {
//		action = menu.addAction(tr("Once"));
//		connect(action, SIGNAL(triggered()), SLOT(scheduleBackup()));
//
//		action = menu.addAction(tr("Weekly"));
//		connect(action, SIGNAL(triggered()), SLOT(scheduleWeeklyBackup()));
	}

	menu.exec(viewport()->mapToGlobal(point));
}

void ScheduleView::scheduleWeeklyBackup() {
	scheduleModel()->append(QString("0 0 * * %1 job").arg((selected_date.dayOfWeek()+1) % 7));
}

void ScheduleView::scheduleDailyBackup() {
	scheduleModel()->append("0 0 * * * job");
}

void ScheduleView::scheduleOneBackup() {
	scheduleModel()->append(QString("0 0 %1 %2 * job").arg(selected_date.day()).arg(selected_date.month()));
}

void ScheduleView::scheduleBackup() {
}
