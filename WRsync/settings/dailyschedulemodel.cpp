#include "dailyschedulemodel.h"

DailyScheduleModel::DailyScheduleModel(QObject *parent) :
	QAbstractProxyModel(parent) {
}

QVariant DailyScheduleModel::data ( const QModelIndex & index, int role) const {

	if (not index.isValid())
		return QVariant();

	int row = index.row();
	int column = index.column();

	switch (role) {
	case Qt::DisplayRole:
		switch (column) {
		case 0:
			return m_schedules[row].cronString();
		case 1:
			return m_schedules[row].commandString();
		default:
			return QVariant();
		}
		default:
		break;
	}

	return QVariant();
}

void DailyScheduleModel::setSourceModel(QAbstractItemModel *sourceModel) {
	QAbstractProxyModel::setSourceModel(sourceModel);
}

void DailyScheduleModel::applyDateFilter() {

	m_source_schedule_indexes = scheduleModel()->matchingIndexes(dateFilter());
	m_schedules = scheduleModel()->matching(dateFilter());

	reset();
}

QModelIndex DailyScheduleModel::mapFromSource ( const QModelIndex & sourceIndex ) const {
	if (not sourceIndex.isValid())
		return QModelIndex();

	if (not m_source_schedule_indexes.contains(sourceIndex.row()))
		return QModelIndex();

	return index(m_source_schedule_indexes.indexOf(sourceIndex.row()), 0);
}

QModelIndex DailyScheduleModel::mapToSource ( const QModelIndex & proxyIndex ) const {
	if (not proxyIndex.isValid())
		return QModelIndex();

	if (proxyIndex.row() >= m_source_schedule_indexes.count())
		return QModelIndex();

	return sourceModel()->index(m_source_schedule_indexes[proxyIndex.row()], 0);
}

QModelIndex DailyScheduleModel::index(int row, int column, const QModelIndex & /* parent */) const {
	return createIndex(row, column, NULL);
}

QModelIndex DailyScheduleModel::parent(const QModelIndex & /* index */) const {
	return QModelIndex();
}
