#pragma once

/** QT headers **/
#include <QAbstractListModel>

/** Application headers **/
#include <interface/processor.h>

class DestinationPluginsModel : public QAbstractListModel {
	Q_OBJECT
	public:
		DestinationPluginsModel(QObject *parent=0);

		/** QT api **/
		Qt::ItemFlags flags ( const QModelIndex & index ) const;

		/** QT api **/
		QVariant data ( const QModelIndex & index, int role ) const;

		/** QT api **/
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

		ProcessorInterface *processor(int row) const;

		int indexOf(QString plugin_id) const;

};
