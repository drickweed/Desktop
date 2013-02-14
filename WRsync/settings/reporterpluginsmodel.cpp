#include "reporterpluginsmodel.h"

/** QT headers **/
#include <QSettings>

/** Application headers **/
#include <wrsync/pluginmanager.h>

using namespace WRsync;

ReporterPluginsModel::ReporterPluginsModel(QObject *parent) :
	QAbstractListModel(parent) {
}

Qt::ItemFlags ReporterPluginsModel::flags(const QModelIndex & index) const {
	return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}

QVariant ReporterPluginsModel::data(const QModelIndex & index, int role) const {
	int row = index.row();

	switch (role) {
		case Qt::DisplayRole:
			if (reporter(row))
				return QVariant(reporter(row)->name());
		case Qt::CheckStateRole:
			return (enabled(reporter(row)->id()) ? Qt::Checked : Qt::Unchecked);

		default:
			return QVariant();
	};

	return QVariant();
}

bool ReporterPluginsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	int row = index.row();

	switch (role) {
		case Qt::CheckStateRole:
			setEnabled(reporter(row)->id(), value.toBool());
			emit dataChanged(index, index);
			return true;
		default:
			break;
	};

	return false;
}

int ReporterPluginsModel::rowCount(const QModelIndex& /** parent **/) const {
	return m_plugins.count();
}

ReporterInterface* ReporterPluginsModel::reporter(int index) const {
	return m_plugins.values()[index];
}

void ReporterPluginsModel::setPlugins(QList<ReporterInterface*> plugins) {
	foreach(ReporterInterface* plugin, plugins) {
		if (plugin)
			m_plugins[plugin->id()] = plugin;
	}
}

void ReporterPluginsModel::savePluginSettings() {
	foreach (ReporterInterface *plugin, m_plugins) {
		plugin->saveConfig();
	}
}

void ReporterPluginsModel::saveSettings(Settings *settings) {
	settings->setEnabledReportPlugins(enabledPlugins());
}

void ReporterPluginsModel::loadSettings(Settings *settings) {
	setEnabledPlugins(settings->enabledReportPlugins());
	reset();
}
