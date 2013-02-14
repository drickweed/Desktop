#include "cdrom.h"

/** QT headers **/
#include <QFile>
#include <QTextStream>
#include <QtPlugin>

/** JB headers **/
#include <jb/std/log.h>

CDWriter::CDWriter():
	QObject() {
	m_process = new QProcess();
	m_genisoimage = new QProcess();

	m_genisoimage->setStandardOutputProcess(m_process);
}

QString CDWriter::commandLine() const {
	return QString("cdrecord %2 tsize=`genisoimage -path-list %1 -q -print-size`s -")
				   .arg(sourceListFile())
				   .arg(dryRun() ? SILENT_SWITCHES :"");
}

QString CDWriter::genIsoImageCommandLine() const {
	return QString("genisoimage -path-list %1 -o - ")
				   .arg(sourceListFile());
}

ProcessorInterface::RunState CDWriter::run() {
	if (m_source_processed)
		return Done;

	m_source_processed = true;

	// Dummy source for logging
	Source source = Source();
	source.setPath("cdrom-backup");

	m_current_result = new Result(source);
	m_results << m_current_result;

	LOG("Running: " << genIsoImageCommandLine().toStdString() << "|" << commandLine().toStdString());

	m_genisoimage->start(genIsoImageCommandLine());
	m_process->start(commandLine());

	return Started;
}

void CDWriter::setSources(QStringList sources) {
	foreach(QString source_path, sources) {
		m_sources << Source(source_path);
	}

	m_source_processed = false;
	generateSourceListFile();
}

QString CDWriter::sourceListFile() const {
	return "record-srouces.txt";
}

void CDWriter::generateSourceListFile() const {
	QFile source_list_file(sourceListFile());
	source_list_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream out(&source_list_file);

	foreach(Source source, m_sources) {
		if (not source.isValid())
			continue;
		out << source.path();
	}

	out.flush();
	source_list_file.close();
}

Q_EXPORT_PLUGIN2(processor_cdwriter, CDWriter)
