#include "copyerrorsmodel.h"

CopyErrorsModel::CopyErrorsModel(QObject *parent):
	QAbstractListModel(parent) {
}


/** QT api **/
Qt::ItemFlags CopyErrorsModel::flags ( const QModelIndex & index ) const {
	return QAbstractListModel::flags(index);
}

void CopyErrorsModel::loadFromLog(QString log_filename) {

}

/** QT api **/
QVariant CopyErrorsModel::data ( const QModelIndex & index, int role) const {
	if (not index.isValid())
		return QVariant();

	int row = index.row();

	switch (role) {
		case Qt::DisplayRole:
			return QVariant(m_errors[row]);
			break;
		default:
			break;
	}

	return QVariant();
}

/** QT api **/
int CopyErrorsModel::rowCount (const QModelIndex & /* parent */) const {
	return m_errors.count();
}
