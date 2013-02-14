#pragma once

/** QT headers **/
#include <QList>
#include <QMap>

/** JB headers **/
#include <jb/calendar/calendar_model.h>

/** Application haeders **/
#include <wrsync/result.h>

using namespace JB;

class CalendarLogModel : public CalendarModel {
	Q_OBJECT
	public:
		CalendarLogModel(QObject *parent=0);

		/** QT Api **/
		QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

	public slots:
		void loadMonthResults(int year, int month);
		void loadMonthResults() { loadMonthResults(currentYear(), currentMonth()); }

	private:
		QMap<QDateTime, int> m_results;
		QMap<QDate, int> m_date_status;

		/**
			Collects all results from a single date.
		*/
		void consolidateResults();

};
