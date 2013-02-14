#pragma once

/** QT headers **/
#include <QObject>
#include <QProcess>
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QWidget>

/** Application headers **/
#include <interface/processor.h>

class Virtual : public QObject ,public ProcessorInterface {
	Q_OBJECT
	Q_INTERFACES(ProcessorInterface)

	public:
		Virtual();

		QString name() const { return "Virtual"; }
		QString id() const { return "894e11ae-2fbc-11df-af13-0016d3298e6d"; }

		ProcessorInterface::RunState run();

		void setSources(QStringList sources);

		QList<Result*> results() const { return m_results; }

		void saveConfig();
		void loadConfig();

		QWidget *configUi();

		QString commandLine(Source source) const;

		QProcess *process() const { return m_process; }

		Result* currentJob() const { return m_current_result; }

		bool dryRun() const { return m_dry_run; }

		void setDryRun(bool dry_run) { m_dry_run = dry_run; }

	private:

		QList<Source> m_sources;

		QList<Result*> m_results;

		QProcess *m_process;

		Result *m_current_result;

		bool m_dry_run;

		QWidget *m_config_dialog;

};
