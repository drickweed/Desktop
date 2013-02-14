#pragma once

/** QT headers **/
#include <QObject>
#include <QString>
#include <QList>

/** Application headers **/
#include <plugins/interface/reporter.h>
#include <plugins/interface/processor.h>
#include <plugins/interface/destination.h>

#include "global.h"

class WRSYNC_API PluginManager {

	public:

		static void loadPlugins();

		static QList<ReporterInterface*> reporterPlugins() { return reporter_plugins; }
		static QList<ProcessorInterface*> processorPlugins() { return processor_plugins; }

		static ReporterInterface* reporter(QObject* plugin);
		static ProcessorInterface* processor(QObject *plugin);

		static ProcessorInterface* processor(QString plugin_id);

	private:
		static QString pluginsPath();

		static QList<ReporterInterface*> reporter_plugins;
		static QList<ProcessorInterface*> processor_plugins;
};

