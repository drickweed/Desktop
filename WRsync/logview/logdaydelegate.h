#pragma once

/** JB headers **/
#include <jb/calendar/calendar_day_delegate.h>

using namespace JB;

class LogDayDelegate : public CalendarDayDelegate {
	Q_OBJECT
	public:
		explicit LogDayDelegate(QWidget *parent=0);

	signals:

	public slots:

};
