#include "settings.h"

/** QT headers **/
#include <QFile>
#include <QTextStream>
#include <QMutableListIterator>

/** JB headers **/
#include <jb/qt4/fileutils.h>
#include <jb/qt4/pathutils.h>

/** Application headers **/
#include "pluginmanager.h"

/** STL headers **/
#include <exception>

using namespace JB;

const QString SOURCES_FILE = "sources.txt";
const QString SCHEDULE_FILE = "schedule.txt";
const QString SETTINGS_FILE = "wrsync.ini";

namespace WRsync {

	Settings::Settings(QString root_path, QObject *parent):
		QSettings(Path::join(root_path, SETTINGS_FILE), QSettings::IniFormat, parent),
		root_path(root_path) {
		loadSources();
		loadSchedule();
	}

	void Settings::loadSources() {
		QFile sources_file(sourcesFile());
		if (not sources_file.open(QIODevice::ReadOnly)) {
			return;
		}

		m_sources = File::readLines(&sources_file);

		sources_file.close();
	}

	void Settings::saveSources() {
		QFile sources_file(sourcesFile());
		if (not sources_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
			throw new ErrorWritingSourcesFile;
		}

		QTextStream out(&sources_file);

		out << m_sources.join("\n");

		sources_file.close();
	}

	void Settings::loadSchedule() {
		QFile schedule_file(scheduleFile());
		if (not schedule_file.open(QIODevice::ReadOnly)) {
			return;
		}

		m_schedules = File::readLines(&schedule_file);

		schedule_file.close();
	}

	void Settings::saveSchedule() {
		QFile schedule_file(scheduleFile());
		if (not schedule_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
			throw new ErrorWritingSourcesFile;
		}

		QTextStream out(&schedule_file);

		out << m_schedules.join("\n");

		schedule_file.close();
	}

	QString Settings::scheduleFile() const {
		return Path::join(root_path, SCHEDULE_FILE);
	}

	QString Settings::sourcesFile() const {
		return Path::join(root_path, SOURCES_FILE);
	}

	QStringList Settings::enabledReportPlugins() const {
		return value("plugins", QStringList()).toStringList();
	}

	void Settings::setEnabledReportPlugins(QStringList enabled_plugins) {
		setValue("plugins", enabled_plugins);
	}

	QString Settings::processorPlugin() const {
		try {
			return value("destination-plugin", PluginManager::processorPlugins()[0]->id()).toString();
		} catch (std::exception *e) {
			return QString();
		}
	}

	void Settings::setProcessorPlugin(QString plugin) {
		setValue("destination-plugin", plugin);
	}

	bool Settings::copyOpenFiles() const {
		return value("copy_open_files", true).toBool();
	}

	void Settings::setCopyOpenFiles(bool copy_open_files) {
		setValue("copy_open_files", copy_open_files);
	}
}
