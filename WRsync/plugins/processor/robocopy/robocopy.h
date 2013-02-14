#pragma once

/** QT headers **/
#include <QObject>
#include <QProcess>
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>

#include <exception>

/** Application headers **/
#include <wrsync/epath.h>
#include <wrsync/result.h>
#include <interface/processor.h>
#include "rsyncconfigdialog.h"
#include "settings.h"

const QString CLI_SWITCHES = "-rv --progress --timeout=60";
const QString SILENT_SWITCHES = "-n";

using namespace std;

class NoDestinationConfigured : public exception {};

namespace RSync {

	class Processor : public QObject, public ProcessorInterface {
		Q_OBJECT
		Q_INTERFACES(ProcessorInterface)

		public:
			Processor();
			~Processor();

			QString name() const { return "Rsync"; }
			QString id() const { return "7310d606-2f99-11df-bf3f-0016d3298e6d"; }

			void report();

			RunState run();

			void setSources(QStringList sources);

			QStringList filters() const;

			QList<Result*> results() const { return m_results; }

			void saveConfig();
			void loadConfig();

			QWidget *configUi();

			QProcess *process() const { return m_process; }

			Result* currentJob() const { return m_current_result; }

			bool dryRun() const { return m_dry_run; }

			void setDryRun(bool dry_run) { m_dry_run = dry_run; }

		private:

			QString commandLine(Source source) const;

			QString m_hostname,
					m_username,
					m_path,
					m_key_path;

			QList<int> m_exit_codes;

			int m_last_result;

			void writeFiltersFile() const;

			bool m_dry_run;

			QString filtersFile() const;

			QList<Source> sources() const;

			QProcess *m_process;

			Result *m_current_result;

			int m_current_source;

			QList<Result*> m_results;

			QList<Source> m_sources;

			RSync::Settings *m_settings;

			RSyncConfigDialog *m_config_ui;

			QString identification() const;

			QString fullPath(QDateTime datetime=QDateTime::currentDateTime()) const;
			QString extendedPath(QDateTime datetime) const;

	};
}
