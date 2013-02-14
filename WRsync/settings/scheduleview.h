#pragma once

/** JB headers **/
#include <jb/calendar/calendar_view.h>

/** Application headers **/
#include <wrsync/schedulemodel.h>

using namespace JB;

class ScheduleView : public CalendarView
{
	Q_OBJECT
	public:
		ScheduleView(QWidget *parent = 0);

	signals:

	private slots:

		void contextMenu(const QPoint & point);

		void scheduleBackup();
		void scheduleWeeklyBackup();
		void scheduleDailyBackup();
		void scheduleOneBackup();

	private:
		ScheduleModel* scheduleModel() const { return qobject_cast<ScheduleModel*>(model()); }

};

