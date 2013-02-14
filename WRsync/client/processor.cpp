#include "processor.h"

/** QT headers **/
#include <QDir>
#include <QMetaType>

/** Application headers **/
#include <wrsync/exceptions.h>
#include <wrsync/logfile.h>

Processor::Processor(ProcessorInterface *processor) :
	QObject(),
	m_last_state(ProcessorInterface::None),
	m_processor(processor) {

	qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
	qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

	connect(process(), SIGNAL(finished(int ,QProcess::ExitStatus)), SLOT(cleanupFinished(int ,QProcess::ExitStatus)), Qt::QueuedConnection);
	connect(process(), SIGNAL(error(QProcess::ProcessError)), SLOT(cleanupError(QProcess::ProcessError)), Qt::QueuedConnection);

	connect(this, SIGNAL(cleanupDone()), SLOT(run()), Qt::QueuedConnection);

	connect(process(), SIGNAL(readyReadStandardError()), SLOT(writeStandardError()), Qt::QueuedConnection);
	connect(process(), SIGNAL(readyReadStandardOutput()), SLOT(writeStandardOutput()), Qt::QueuedConnection);

	connect(this, SIGNAL(done()), SLOT(jobCleanup()));
}

void Processor::openLogs() {
	openLog();
	openErrorLog();
}

void Processor::openLog() {
	m_log.setFileName(QString("%1.log").arg(processor()->currentResult()->logPath()));

	LAPP_ << "Opening log file: " << QSTR(m_log.fileName());

	if (not m_log.open(QIODevice::WriteOnly))
		throw new ErrorOpeningLogFile(m_log.fileName());

	m_log_out.setDevice(&m_log);
}

void Processor::openErrorLog() {
	m_error_log.setFileName(QString("%1 - errors.log").arg(processor()->currentResult()->logPath()));

	LAPP_ << "Opening error log file: " << QSTR(m_error_log.fileName());

	if (not m_error_log.open(QIODevice::WriteOnly))
		throw new ErrorOpeningLogFile(m_error_log.fileName());

	m_error_log_out.setDevice(&m_error_log);
}

void Processor::start() {

	try {
		if (not processor()->backupPossible())
			return;
	} catch (ErrorCreatingDestinationPath *e ) {
		m_last_state = ProcessorInterface::MissingDestination;
	} catch (DestinationPathMissing *e) {
		m_last_state = ProcessorInterface::MissingDestination;
	} catch (DestinationDeviceMissing *e) {
		m_last_state = ProcessorInterface::MissingDestination;
	} catch (exception *e) {
		LERROR_ << "Unknown exception occured";
		m_last_state = ProcessorInterface::Error;
	}

	if (m_last_state != ProcessorInterface::None) {
		processState();
		return;
	}

	run();
}

void Processor::closeLogs() {
	m_log_out.flush();
	m_log.close();

	m_error_log_out.flush();
	m_error_log.close();
}

void Processor::run() {

	try {
		m_last_state = processor()->run();
	} catch (ErrorInvalidSource *e) {
		m_last_state = ProcessorInterface::InvalidSource;
	} catch (ErrorLocatingRemovableDevice *e) {
		m_last_state = ProcessorInterface::InvalidSource;
	}

	processState();
}

void Processor::processState() {

	switch (m_last_state) {
		case ProcessorInterface::Done:
			LAPP_ << "Finished processing all sources";
			emit done();
			break;
		case ProcessorInterface::FailedToStart:
			LERROR_ << "Single source failed to backup";
			processor()->setResultState(Result::FailedToStart);
			finalizeResult();
			break;
		case ProcessorInterface::InvalidSource:
			processor()->setResultState(Result::MissingSource);
			finalizeResult();
			break;
		case ProcessorInterface::MissingDestination:
			LERROR_ << "Missing destination path";
			processor()->initResult(EPath());
			processor()->setResultState(Result::MissingDestination);
			emit done();
			break;
		case ProcessorInterface::Started:
			LAPP_ << "Backup started properly";
			LAPP_ << "Opening process log files";
			openLogs();
			break;
		case ProcessorInterface::Error:
			LERROR_ << "Unhandled error";
			finalizeResult();
			break;
		case ProcessorInterface::None:
			break;
	}

}

void Processor::jobCleanup() {
	processor()->jobCleanup();
}

void Processor::cleanupError(QProcess::ProcessError error) {
	LAPP_ << "Error while starting copy process. (QProcess error code: " << error << ")";
	processor()->setResultState(Result::FailedToStart);
	finalizeResult();
}

void Processor::abort() {
	processor()->abort();
}

void Processor::finalizeResult() {
	closeLogs();
	processor()->cleanupSource();
	emit cleanupDone();
}

void Processor::processUserRequests() {
	QFile abort_request("temp/abort.sig");
	if (abort_request.exists()) {
		abort_request.remove();
		abort();
	}
}

void Processor::cleanupFinished(int exitCode, QProcess::ExitStatus exitStatus) {

	if (exitStatus == QProcess::NormalExit) {
		processor()->currentResult()->setExitCode(exitCode);
		LAPP_ << "Copy result: " << exitCode;
	} else if (processor()->currentResultState() != Result::Aborted) {
		processor()->setResultState(Result::Crashed);
		LAPP_ << "Copy result: crashed";
	}
	finalizeResult();
}

void Processor::ensureBaseTempPath() {
	QDir::current().mkpath(TEMP_PATH);
}

void Processor::writeStandardError() {
	m_error_log_out << process()->readAllStandardError();
	m_error_log_out.flush();
}

void Processor::writeStandardOutput() {
	m_log_out << process()->readAllStandardOutput();
	m_log_out.flush();
}
