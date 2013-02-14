#pragma once

/** QT headers **/
#include <QObject>
#include <QProcess>
#include <QFile>
#include <QTextStream>

/** Application headers **/
#include <plugins/interface/processor.h>

/** STL headers **/
#include <exception>

const QString TEMP_PATH = "temp";

/**
	Communication wrapper for processor plugins.
*/
class Processor : public QObject {
	Q_OBJECT
	public:
		Processor(ProcessorInterface *processor);

		ProcessorInterface *processor() const { return m_processor; }

		/**
			Ensure that the path for temporary files exists
		*/
		static void ensureBaseTempPath();

		void start();

	public slots:
		void abort();

	private slots:
		void run();

	private:

		QFile m_log,
			  m_error_log;

		QTextStream m_log_out,
					m_error_log_out;

		void openErrorLog();
		void openLog();

		void openLogs();
		void closeLogs();

		void processState();

		ProcessorInterface::RunState m_last_state;

		/** Backup processor **/
		ProcessorInterface *m_processor;

		QProcess *process() const { return m_processor->process(); }

		void finalizeResult();

	public slots:
		void processUserRequests();

		void jobCleanup();

	private slots:

		void cleanupFinished(int exitCode, QProcess::ExitStatus exitStatus);
		void cleanupError(QProcess::ProcessError error);

		/** Logging **/
		void writeStandardError();
		void writeStandardOutput();

	signals:

		void copyDone();
		void cleanupDone();
		void done();
};
