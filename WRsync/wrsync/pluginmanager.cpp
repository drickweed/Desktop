#include "pluginmanager.h"

/** QT headers **/
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>

/** JB headers **/
#include <jb/qt4/pathutils.h>

#include "logfile.h"

using namespace JB;

QList<ReporterInterface*> PluginManager::reporter_plugins;
QList<ProcessorInterface*> PluginManager::processor_plugins;

void PluginManager::loadPlugins() {
	const QDir plugins_dir(pluginsPath());
	QStringList file_names = plugins_dir.entryList(QStringList() << "*", QDir::Files, QDir::Name);
	foreach(QString file_name, file_names) {

		LDEBUG_ << "Attempting to load plugin from " << QSTR(file_name);
		QPluginLoader loader(plugins_dir.absoluteFilePath(file_name));

		QObject *plugin = loader.instance();

		if (plugin) {

			LDEBUG_ << "Checking plugin type";

			ReporterInterface *reporter_p = reporter(plugin);
			if (reporter_p) {
				reporter_plugins.append(reporter_p);
				LAPP_ << "Found reporter plugin " << QSTR(reporter_p->name()) << " (" << QSTR(file_name) << ")";
				continue;
			}

			ProcessorInterface *processor_p = processor(plugin);
			if (processor_p) {
				processor_plugins.append(processor_p);
				LAPP_ << "Found processor plugin " << QSTR(processor_p->name()) << " (" << QSTR(file_name) << ")";
				continue;
			}
		}
	}
}

QString PluginManager::pluginsPath() {
	return Path::join(qApp->applicationDirPath(), "plugins");
}

ReporterInterface* PluginManager::reporter(QObject* plugin) {
	return qobject_cast<ReporterInterface*>(plugin);
}

ProcessorInterface* PluginManager::processor(QObject *plugin) {
	return qobject_cast<ProcessorInterface*>(plugin);
}

ProcessorInterface* PluginManager::processor(QString plugin_id) {
	foreach(ProcessorInterface *plugin, processor_plugins) {
		if (plugin && plugin->id() == plugin_id)
			return plugin;
	}
	return NULL;
}
