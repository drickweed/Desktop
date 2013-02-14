#include "report.h"

/** Application headers **/
#include <plugins/interface/reporter.h>

/** JB headers **/
#include <jb/std/log.h>

Report::Report(Settings *settings, QObject *parent):
	QObject(parent),
	m_settings(settings) {
}

QList<ReporterInterface*> Report::activeReporterPlugins() const {
	QList<ReporterInterface*> active_plugins;
	QStringList active_plugin_ids = m_settings->enabledReportPlugins();
	foreach(ReporterInterface *plugin, PluginManager::reporterPlugins()) {
		if (active_plugin_ids.contains(plugin->id())) {
			active_plugins.append(plugin);
		}
	}
	return active_plugins;
}

void Report::reportUsingPlugins(Result *result) {

	foreach(ReporterInterface *plugin, activeReporterPlugins()) {
		if (not plugin->report(*result)) {
			LOG("Error reporting using " << plugin->name().toStdString());
		}
	}

	emit done();
}
