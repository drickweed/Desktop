#pragma once

/** QT headers **/
#include <QTableView>

class DailyScheduleView : public QTableView {
	Q_OBJECT
	public:
		DailyScheduleView(QWidget *parent=0);

		QModelIndex selectedRow() const;
	signals:

	public slots:

};
