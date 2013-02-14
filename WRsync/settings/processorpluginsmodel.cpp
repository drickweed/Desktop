#include "processorpluginsmodel.h"

#include "wrsync/pluginmanager.h"

ProcessorPluginsModel::ProcessorPluginsModel(QObject *parent) :
	QAbstractListModel(parent) {
}

QVariant ProcessorPluginsModel::data(const QModelIndex & index, int role) const {
	int row = index.row();

	switch (role) {
		case Qt::DisplayRole:
			if (processor(row))
				return QVariant(processor(row)->name());
		default:
			return QVariant();
	};

	return QVariant();
}

void ProcessorPluginsModel::savePluginSettings() {
	foreach (ProcessorInterface *plugin, m_plugins) {
		plugin->saveConfig();
	}
}

void ProcessorPluginsModel::setPlugins(QList<ProcessorInterface*> plugins) {
	foreach (ProcessorInterface* plugin, plugins) {
		if (plugin) {
			m_plugins.append(plugin);
			m_plugin_ids[plugin->id()] = m_plugins.indexOf(plugin);
		}
	}
}

int ProcessorPluginsModel::indexOf(QString plugin_id) const {
	return m_plugin_ids[plugin_id];
}

int ProcessorPluginsModel::rowCount(const QModelIndex& /** parent **/) const {
	return m_plugins.count();
}

ProcessorInterface* ProcessorPluginsModel::processor(int index) const {
	return m_plugins[index];
}
