#include "report.h"

/** Application headers **/
#include <plugins/interface/reporter.h>
#include <wrsync/logfile.h>

Report::Report(Processor *processor, Settings *settings, QObject *parent):
	QObject(parent),
	m_settings(settings),
	m_processor(processor) {
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

void Report::reportUsingPlugins() {
	LAPP_ << "Sending " << m_processor->processor()->results().count() << " results";

	foreach(Result* result, m_processor->processor()->results()) {
		foreach(ReporterInterface *plugin, activeReporterPlugins()) {
			LAPP_ << "Reporting using " << QSTR(plugin->name());
			if (not plugin->report(*result)) {
				LAPP_ << "Error reporting using " << QSTR(plugin->name());
			}
		}
	}

	LAPP_ << "Done reporting";
	emit done();
}

ReporterInterface* Report::firstReporter() const {
	if (m_settings->enabledReportPlugins().empty())
		return NULL;
	return activeReporterPlugins()[0];
}
