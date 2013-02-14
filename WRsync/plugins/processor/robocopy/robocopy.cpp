#include "rsync.h"

/** QT headers **/
#include <QDir>
#include <QtPlugin>
#include <QByteArray>

/** JB headers **/
#include <jb/std/log.h>
#include <jb/qt4/stringutils.h>

using namespace JB;

namespace RSync {

	Processor::Processor():
		QObject(),
		m_last_result(-1),
		m_config_ui(NULL) {

		m_process = new QProcess(this);
		m_settings = new RSync::Settings(this);

		loadConfig();
	}

	Processor::~Processor() {
		saveConfig();
		delete m_process;
		delete m_settings;
	}

	QStringList Processor::filters() const {
		QStringList rsync_filters;
		foreach(Source source, m_sources) {
			if (source.type() != Source::Path)
				rsync_filters << source.path();
		}

		return rsync_filters;
	}

	ProcessorInterface::RunState Processor::run() {
		LOG("Running: " << name().toStdString());

		if (m_current_source >= sources().count())
			return ProcessorInterface::Done;

		Source source = sources()[m_current_source];
		LOG("Processing source: (" << m_current_source << ")" << source.path().toStdString());
		m_current_result = new Result(source);
		m_results << m_current_result;

		m_current_source++;

		if (not source.isValid()) {
			LOG("Invalid source path specified" << source.path().toStdString());
			return ProcessorInterface::FailedToStart;
		}

		LOG("Running: " << commandLine(source.path()).toStdString());
		m_process->start(commandLine(source.path()));

//		if (not m_settings->password().isEmpty()) {
//			QByteArray password = m_settings->password().toAscii();
//			m_process->write(password);
//		}

		return ProcessorInterface::Started;
	}

	void Processor::setSources(QStringList sources) {
		foreach(QString source_path, sources) {
			m_sources << Source(source_path);
		}

		m_current_source = 0;

		writeFiltersFile();
	}

	QString Processor::identification() const {
		if (m_settings->keyPath().isEmpty())
			return "";

		return QString("-e \"ssh -i %1\"").arg(m_settings->keyPath());
	}

	QString Processor::commandLine(Source source) const {
		QString command_line;

		command_line = QString("rsync %1 %2 --include-from=%3 %4 %5 %6")
					   .arg(CLI_SWITCHES)
					   .arg(dryRun() ? SILENT_SWITCHES : "")
					   .arg(String::quoteEscape(filtersFile()))
					   .arg(identification())
					   .arg(String::quoteEscape(source.unixPath()))
					   .arg(String::quoteEscape(fullPath(currentJob()->startedAt())));

		return command_line;

	}

	QString Processor::filtersFile() const {
		return QString("RENAME_ME.filters");
	}

	void Processor::loadConfig() {
		LOG("Loading rsync plugin settings");
		m_hostname = m_settings->hostname();
		m_username = m_settings->username();
		m_path = m_settings->path();
		m_key_path = m_settings->keyPath();
	}

	void Processor::saveConfig() {
		if (m_config_ui == NULL)
			return;

		LOG("Saving rsync plugin settings");
		m_config_ui->saveConfig(m_settings);
	}

	QWidget* Processor::configUi() {
		if (m_config_ui == NULL)
			m_config_ui = new RSyncConfigDialog();

		LOG("Loading rsync plugin settings into manager");
		m_config_ui->loadConfig(m_settings);
		return m_config_ui;
	}

	void Processor::writeFiltersFile() const {
		QFile filters_file(filtersFile());
		filters_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		QTextStream out(&filters_file);
		out << filters().join("\n");
		filters_file.close();
	}

	QList<Source> Processor::sources() const {
		QList<Source> p_sources;
		foreach(Source source, m_sources) {
			if (source.type() == Source::Path)
				p_sources << source;
		}

		return p_sources;
	}

	QString Processor::fullPath(QDateTime datetime) const {
		QString full_path;

		if (m_path.isEmpty())
			throw new NoDestinationConfigured();

		if (not m_hostname.isEmpty())
			full_path = QString("%1@%2:/%3").arg(m_username).arg(m_hostname).arg(extendedPath(datetime));
		else
			full_path = QString("%1").arg(m_path);

		return full_path;
	}

	QString Processor::extendedPath(QDateTime datetime) const {
		QString e_path = m_path;

		e_path = e_path.replace("%D", QDate::longDayName(datetime.date().dayOfWeek()));

		return e_path;
	}

}

Q_EXPORT_PLUGIN2(processor_rsync, RSync::Processor)
