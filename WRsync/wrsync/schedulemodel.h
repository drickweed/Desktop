#pragma once

/** QT headers **/
#include <QAbstractTableModel>
#include <QList>

/** JB headers **/
#include <jb/calendar/calendar_model.h>

#include "schedule.h"

using namespace JB;

class ScheduleModel : public CalendarModel {
	Q_OBJECT
	public:
		ScheduleModel(QObject *parent=0);

		/** QT Api **/
		QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

		void readSchedules(QStringList schedules);
		QStringList writeSchedules();

		void append(QString schedule);
		void append(Schedule schedule);

		void removeAt(QModelIndex index);

		QList<Schedule> matching(QDate date) const;
		QList<int> matchingIndexes(QDate date) const;

		Schedule at(QModelIndex index) const;

		int scheduleCount() const { return m_schedules.count(); }

	private:
		QList<Schedule> m_schedules;

	signals:
		void scheduleRemoved();

	public slots:

};
