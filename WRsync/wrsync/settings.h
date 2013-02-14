#pragma once

/** QT headers **/
#include <QSettings>
#include <QString>
#include <QStringList>

/** STL headers **/
#include <exception>

#include "global.h"

using namespace std;

class ErrorOpeningSourcesFile: public exception {

};

class ErrorWritingSourcesFile: public exception {

};

namespace WRsync {

	class WRSYNC_API Settings : public QSettings {
		Q_OBJECT
		public:
			Settings(QString root_path=QString(), QObject *parent=0);

			QStringList sources() const { return m_sources; }
			void setSources(QStringList sources) { m_sources = sources; }

			QStringList schedules() const { return m_schedules; }
			void setSchedules(QStringList schedules) { m_schedules = schedules; }

			void sync() {
				QSettings::sync();
				saveSources();
				saveSchedule();
			}

			QString processorPlugin() const;
			void setProcessorPlugin(QString plugin);

			QStringList enabledReportPlugins() const;
			void setEnabledReportPlugins(QStringList enabled_plugins);

			bool copyOpenFiles() const;
			void setCopyOpenFiles(bool copy_open_files);

		private:

			void loadSources();
			void saveSources();

			void loadSchedule();
			void saveSchedule();

			QStringList m_sources;
			QStringList m_schedules;

			QString root_path;

			QString scheduleFile() const;
			QString sourcesFile() const;

		signals:

		public slots:

	};
}
