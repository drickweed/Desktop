#include "calendarlogmodel.h"

/** Application headers **/
#include <wrsync/pluginmanager.h>

/** QT headers **/
#include <QColor>
#include <QBrush>
#include <QDebug>

CalendarLogModel::CalendarLogModel(QObject *parent) :
	CalendarModel(parent) {
}

QVariant CalendarLogModel::data(const QModelIndex &index, int role) const {

	switch(role) {
		case Qt::BackgroundRole:
			if (m_date_status.value(date(index), Result::None) & Result::AnyError) {
				return QVariant(QColor(Qt::red));
			}
			if (m_date_status.value(date(index), Result::None) & Result::OK) {
				return QVariant(QColor(Qt::green));
			} else

			break;
		default:
			break;

	}

	return CalendarModel::data(index, role);
}

void CalendarLogModel::loadMonthResults(int year, int month) {
	m_results.clear();

	foreach(ReporterInterface* plugin, PluginManager::reporterPlugins()) {
		if (plugin) {
			m_results.unite(plugin->resultsForMonth(month, year));
		}
	}

	consolidateResults();
	reset();
}

void CalendarLogModel::consolidateResults() {
	m_date_status.clear();
	foreach(QDateTime result_date_time, m_results.keys()) {
		QDate date = result_date_time.date();
		m_date_status[date] |= m_results[result_date_time];
	}
}
