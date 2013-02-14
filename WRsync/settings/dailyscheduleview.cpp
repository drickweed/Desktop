#include "dailyscheduleview.h"

#include <QModelIndexList>

DailyScheduleView::DailyScheduleView(QWidget *parent) :
	QTableView(parent) {
}

QModelIndex DailyScheduleView::selectedRow() const {
	QModelIndexList selection = selectedIndexes();
	if (selection.isEmpty())
		return QModelIndex();

	return selection[0];
}
