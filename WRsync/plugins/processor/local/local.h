#pragma once

/** QT headers **/
#include <QObject>
#include <QProcess>
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

/** JB headers **/
#include <jb/qt4/pathutils.h>
#include <jb/qt4/strings.h>

/** STL headers **/
#include <exception>

/** Application headers **/
#include <wrsync/epath.h>
#include <wrsync/result.h>
#include <interface/processor.h>
#include <interface/reporter.h>
#include "configdialog.h"
#include "settings.h"
#include <wrsync/logfile.h>

/** Command line switches **/
const QString CLI_SWITCHES = "-rv"; // FIXME: add more valid switches (summary report, workarounds etc.)
const QString SILENT_SWITCHES = "-n";
const QString FILTERS_FILE = "temp/rsync.filters";

using namespace std;
using namespace JB;

class NoDestinationConfigured : public exception {};

namespace Local {

	class Processor : public QObject, public ProcessorInterface {
		Q_OBJECT
		Q_INTERFACES(ProcessorInterface)

		public:
			Processor();
			~Processor();

			QString name() const { return "Local"; }
			QString id() const { return "7310d606-2f99-11df-bf3f-0016d3298e6e"; }

			void report();

			RunState run();

			void abort();

			bool backupPossible();

			void setSources(QStringList sources);

			QStringList filters() const;

			QList<Result*> results() const { return m_results; }

			void saveConfig();
			void loadConfig();

			QWidget *configUi();

			QProcess *process() const { return m_process; }

			Result* currentResult() const;

			bool dryRun() const { return m_dry_run; }

			void setDryRun(bool dry_run) { m_dry_run = dry_run; }

			void initResult(EPath source);

			void setResultState(Result::State state);

			Result::State currentResultState() const;

			bool jobCleanup();

			void cleanupSource();

			void setReporterPlugin(ReporterInterface *plugin) { m_reporter = plugin; }

		private:

			EPath m_destination_path;
			QList<int> m_exit_codes;
			int m_last_result;
			bool m_dry_run;
			QProcess *m_process;
			Result *m_current_result;
			int m_current_source_index;
			QList<Result*> m_results;
			QList<EPath> m_sources;
			Local::Settings *m_settings;
			Local::ConfigDialog *m_config_ui;
			EPath m_source;
			QStringList m_filters;
			QDateTime m_current_job_date_time;
			EPath m_last_destination_path;
			ReporterInterface *m_reporter;

		private: // Methods

			bool mountNetworkSources();
			bool unmountNetworkSources();

			void writeFiltersFile() const;

			QString destinationCygwinPath(EPath source) const {
				return QDir::fromNativeSeparators(QString("%1/%2")
					.arg(expandPath(m_destination_path.unixPath()),
						 Path::usableFileName(source.realPath()))); }

			QString destinationLocalPath(EPath source) const {
				return QDir::toNativeSeparators(QString("%1/%2")
					.arg(expandPath(m_destination_path.realPath()),
						 Path::usableFileName(source.realPath()))); }

			QString expandPath(QString path, QDateTime datetime) const;

			QString expandPath(QString path) const {
				return expandPath(path, m_current_job_date_time);
			}

			QString destinationLocalPath() const {
				return expandPath(m_destination_path.path());
			}

			bool ensureDestinationPath() const {
				LPLUGIN_ << "Ensuring destination path exists " << QSTR(destinationLocalPath());

				if (QDir(destinationLocalPath()).exists())
					return true;

				return QDir::current().mkpath(destinationLocalPath());
			}

			EPath prepareNextSource();

			bool allSourcesProcessed() const { return m_current_source_index >= sources().count(); }

			QString filtersFile() const;

			QList<EPath> sources() const;

			QStringList commandLineArgs(EPath source) const;

			void setLastDestinationPath(EPath path) { m_last_destination_path = path; }

			/**
				Command line switches needed to hard link the previous backup to the current one.
			*/
			QString linkDestination(EPath source) const;

	};
}
