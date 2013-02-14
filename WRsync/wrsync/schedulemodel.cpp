#include "schedulemodel.h"

/** QT headers **/
#include <QColor>
#include <QImage>

#include <wrsync/logfile.h>

ScheduleModel::ScheduleModel(QObject *parent) :
		CalendarModel(parent) {
}

QVariant ScheduleModel::data(const QModelIndex &index, int role) const {

	if (not index.isValid())
		return QVariant();

	switch (role) {
		case Qt::DecorationRole:
			if (not matching(date(index)).isEmpty())
				return QVariant(QImage(":/icons/blue-dot.png"));
			break;
		default:
			break;
	}

	return CalendarModel::data(index, role);

}

void ScheduleModel::readSchedules(QStringList schedules) {
	Schedule schedule;
	foreach (QString cron_string, schedules) {
		schedule.build(cron_string);
		m_schedules.append(schedule);
	}
	reset();
}

void ScheduleModel::append(QString schedule_str) {
	LAPP_ << "Scheduling backup job: " << QSTR(schedule_str);

	Schedule s;
	s.build(schedule_str);
	append(s);
}

void ScheduleModel::append(Schedule schedule) {
	m_schedules.append(schedule);
	reset();
}

void ScheduleModel::removeAt(QModelIndex index) {
	m_schedules.removeAt(index.row());
	emit scheduleRemoved();
	reset();
}

QStringList ScheduleModel::writeSchedules() {
	LAPP_ << "Saving job schedules";
	QStringList cron_lines;
	foreach(Schedule schedule, m_schedules)
		cron_lines.append(schedule.toString());

	LDEBUG_ << "Saved " << cron_lines.count() << " schedules";
	return cron_lines;
}

QList<Schedule> ScheduleModel::matching(QDate date) const {
	QList<Schedule> schedules;
	foreach (Schedule schedule, m_schedules)
		if (schedule.matches(date))
			schedules.append(schedule);
	return schedules;
}

QList<int> ScheduleModel::matchingIndexes(QDate date) const {
	QList<int> list;
	for (int i=0; i < scheduleCount(); i++)
		if (m_schedules[i].matches(date))
			list.append(i);
	return list;
}

Schedule ScheduleModel::at(QModelIndex index) const {
	return m_schedules[index.row()];
}
