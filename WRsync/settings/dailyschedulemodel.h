#pragma once

/** QT headers **/
#include <QAbstractProxyModel>
#include <QList>
#include <QModelIndex>

/** Application headers **/
#include "wrsync/schedule.h"
#include "wrsync/schedulemodel.h"

class DailyScheduleModel : public QAbstractProxyModel {
	Q_OBJECT
	public:
		DailyScheduleModel(QObject *parent=0);

		/** QT api **/
		QVariant data ( const QModelIndex & index, int role=Qt::DisplayRole ) const;

		/** QT api **/
		int rowCount ( const QModelIndex & /* parent */ = QModelIndex() ) const { return m_schedules.count(); }

		/** QT api **/
		int columnCount ( const QModelIndex & /* parent */ = QModelIndex() ) const { return 2; }

		ScheduleModel* scheduleModel() const { return qobject_cast<ScheduleModel*>(sourceModel()); }

		void setDateFilter(QDate date) {
			m_date_filter = date;
			applyDateFilter();
		}

		/** QT api **/
		void setSourceModel(QAbstractItemModel * sourceModel);

		QDate dateFilter() const { return m_date_filter; }

		/** QT api **/
		QModelIndex mapFromSource ( const QModelIndex & sourceIndex ) const;

		/** QT api **/
		QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;

		/** QT api **/
		QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;

		/** QT api **/
		QModelIndex parent(const QModelIndex &index) const;

	private:
		QDate m_date_filter;
		QList<int> m_source_schedule_indexes;
		QList<Schedule> m_schedules;


	private: // Methods
		void applyDateFilter();

};
