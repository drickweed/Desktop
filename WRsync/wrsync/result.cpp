#include "result.h"

/** QT headers **/
#include <QDir>
#include <QTextStream>
#include <QBuffer>
#include <QXmlStreamWriter>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** Application headers **/
#include <wrsync/exceptions.h>

using namespace JB;

int Result::AnyError = Result::Error | Result::Crashed | Result::FailedToStart | Result::MissingDestination | Result::MissingSource | Result::UnknownError | Result::Aborted;

Result::Result() {
	init();
}

Result::Result(EPath source):
	m_source(source){
	init();
}

void Result::init() {
	m_exit_code = 0; // Default exit code
	m_state = None;
	m_started_at = QDateTime::currentDateTime();
}

void Result::setSourcePath(EPath source) {
	m_source = source;
}

void Result::setSourcePath(QString source) {
	m_source = EPath(source);
}

void Result::ensureBaseLogPath() {
	if (not QDir::current().mkpath(LOG_PATH))
		throw new ErrorCreatingLogPath;
}

QString Result::logPath() const {
	QString log_file_name = QString("%1 - %2").arg(m_source.realPath()).arg(startedAt().toString("dd-MM-yyyy hh:mm:ss"));
	return QDir::toNativeSeparators(QString("%1/%2").arg(LOG_PATH).arg(Path::usableFileName(log_file_name)));
}

void Result::parseLogs() {

}

void Result::setExitCode(int exit_code) {
	m_exit_code = exit_code;

	if (exit_code == 0)
		setState(Result::OK);
	else
		setState(Result::Error);
}

QByteArray Result::toXml() const {

	QBuffer xml;
	QXmlStreamWriter writer(&xml);

	writer.writeStartDocument("1.0");

	writer.writeStartElement("started_at");
	writer.writeCDATA(startedAt().toString());
	writer.writeEndElement();

	writer.writeStartElement("state");
	writer.writeCDATA(QString("$1").arg(state()));
	writer.writeEndElement();

	writer.writeStartElement("exit_code");
	writer.writeCDATA(QString("$1").arg(exitCode()));
	writer.writeEndElement();

	writer.writeStartElement("source");
	writer.writeCDATA(sourcePath());
	writer.writeEndElement();

	writer.writeStartElement("destination_path");
	writer.writeCDATA(destination().realPath());
	writer.writeEndElement();

	writer.writeStartElement("destination_id");
	writer.writeCDATA(destination().driveID());
	writer.writeEndElement();

	writer.writeEndDocument();

	return xml.data();
}
