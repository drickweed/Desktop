#include "destinationpluginsmodel.h"

/** Application headers **/
#include <wrsync/pluginmanager.h>

DestinationPluginsModel::DestinationPluginsModel(QObject *parent) :
	QAbstractListModel(parent) {
}

/** QT api **/
Qt::ItemFlags DestinationPluginsModel::flags ( const QModelIndex & index ) const {
	return QAbstractListModel::flags(index);
}

/** QT api **/
QVariant DestinationPluginsModel::data ( const QModelIndex & index, int role ) const {

	int row = index.row();

	switch (role) {
		case Qt::DisplayRole:
			return QVariant(PluginManager::processorPlugins()[row]->name());

		default:
			return QVariant();
	};

	return QVariant();
}

int DestinationPluginsModel::indexOf(QString plugin_id) const {
	for (int i=0; i<PluginManager::processorPlugins().count(); i++) {
		if (PluginManager::processorPlugins()[i]->id() == plugin_id)
			return i;
	}
	return -1;
}

/** QT api **/
int DestinationPluginsModel::rowCount(const QModelIndex& /* parent */) const {
	return PluginManager::processorPlugins().count();
}

ProcessorInterface* DestinationPluginsModel::processor(int row) const {
	return PluginManager::processorPlugins()[row];
}
