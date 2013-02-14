#pragma once

/** QT headers **/
#include <QObject>

/** Application headers **/
#include <interface/processor.h>

const QString SILENT_SWITCHES = "-dummy";

class CDWriter : public QObject, public ProcessorInterface {
	Q_OBJECT
	Q_INTERFACES(ProcessorInterface)

	public:
		CDWriter();

		RunState run();

		QString name() const { return "CD Recorder"; }
		QString id() const { return "493ac8a6-2fc5-11df-98d5-0016d3298e6d"; }

		void setSources(QStringList sources);

		QStringList filters() const;

		QList<Result*> results() const { return m_results; }

		void saveConfig() {}
		void loadConfig() {}

		QWidget *configUi() { return new QWidget(); }

		QProcess *process() const { return m_process; }

		Result* currentJob() const { return m_current_result; }

		bool dryRun() const { return m_dry_run; }

		void setDryRun(bool dry_run) { m_dry_run = dry_run; }

	private:
		QString commandLine() const;

		QString genIsoImageCommandLine() const;

		QString sourceListFile() const;

		void generateSourceListFile() const;

		QList<int> m_exit_codes;

		int m_last_result;

		void writeFiltersFile() const;

		bool m_dryrun;

		QString filtersFile() const;

		QList<Source> sources() const;

		QProcess *m_process,
				 *m_genisoimage;

		Result *m_current_result;

		QList<Result*> m_results;

		QList<Source> m_sources;

		bool m_source_processed;

		bool m_dry_run;
};
