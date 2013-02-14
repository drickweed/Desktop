#include "local.h"

/** QT headers **/
#include <QDir>
#include <QtPlugin>
#include <QByteArray>
#include <QLocale>
#include <QHostInfo>

/** Application headers **/
#include <wrsync/exceptions.h>

class NoCurrentJob : public exception {};

using namespace JB;

namespace Local {

	Processor::Processor():
		QObject(),
		m_last_result(-1),
		m_current_result(NULL),
		m_config_ui(NULL) {

		m_process = new QProcess(this);
		m_settings = new Local::Settings(this);
		m_current_job_date_time = QDateTime::currentDateTime();

		loadConfig();
	}

	Result* Processor::currentResult() const {
		if (m_current_result == NULL) {
			LERROR_ << "No result created";
			throw new NoCurrentJob();
		}

		return m_current_result;
	}

	Processor::~Processor() {
		LPLUGIN_ << QSTR(name()) << " Saving settings";
		saveConfig();
	}

	QStringList Processor::filters() const {
		return m_filters;
	}

	EPath Processor::prepareNextSource() {
		EPath source = sources()[m_current_source_index];

		LPLUGIN_ << "Processing source: (index: " << m_current_source_index << ") " << QSTR(source.path());

		LPLUGIN_ << "Initializing result report";
		initResult(source);

		m_current_source_index++;

		if (not source.isValid()) {
			LPLUGIN_ << "Invalid source path specified " << QSTR(source.path());
			setResultState(Result::MissingSource);
			throw new ErrorInvalidSource();
		}

		if (source.isNetworkPath()) {
			LPLUGIN_ << "Source is a network path";
			if (not source.mountNetwork()) {
				LPLUGIN_ << "Failed to mount network path";
				throw new ErrorInvalidSource();
			}
			LPLUGIN_ << "Mounted network path " << QSTR(source.realPath()) <<" to " << QSTR(source.path());
		}

		LPLUGIN_ << "Finding previous backup destination";
		if (m_reporter)
			setLastDestinationPath(m_reporter->findLastValidForSource(source.realPath(), m_destination_path.driveID()));

		return source;
	}

	void Processor::cleanupSource() {
		if (m_source.isNetworkPath()) {
			if (not m_source.unmountNetwork()) {
				LPLUGIN_ << "Failed to unmount source network path " << QSTR(m_source.realPath());
				return;
			}
			LPLUGIN_ << "Unmounted source network path " << QSTR(m_source.realPath());
		}
	}

	void Processor::initResult(EPath source) {
		m_current_result = new Result(source);
		m_results << m_current_result;
		EPath real_destination(expandPath(destinationLocalPath(source)));
		m_current_result->setDestination(real_destination);
	}

	void Processor::setResultState(Result::State state) {
		if (m_current_result)
			m_current_result->setState(state);
	}

	Result::State Processor::currentResultState() const {
		if (m_current_result == NULL)
			return Result::None;
		return m_current_result->state();
	}

	bool Processor::mountNetworkSources() {
		if (m_destination_path.isNetworkPath())
			if (not m_destination_path.mountNetwork()) {
				LPLUGIN_ << "Error mounting network destination path";
				return false;
			}
		return true;
	}

	bool Processor::unmountNetworkSources() {
		if (m_destination_path.isNetworkPath())
			if (not m_destination_path.unmountNetwork()) {
				LPLUGIN_ << "Error unmounting network destination path";
				return false;
			}
		return true;
	}

	bool Processor::backupPossible() {

		// Check destination

		if (m_destination_path.isEmpty()) {
			LERROR_ << "Destination path not set";
			throw new DestinationPathMissing();
		}

		LPLUGIN_ << "Finding destination path";
		if (m_settings->attachRemovable() and not m_destination_path.attachToDevice(m_settings->ids())) {
			LPLUGIN_ << "Could not find valid device to use as backup";
			throw new DestinationDeviceMissing();
		}

		LPLUGIN_ << "Destination path set to " << QSTR(destinationLocalPath());

		if (not dryRun())
			if (m_destination_path.isLocalPath() and not ensureDestinationPath()) {
				LERROR_ << "Failed to create destination path";
				throw new ErrorCreatingDestinationPath();
			}

		if (m_destination_path.isLocalPath() and not QDir(destinationLocalPath()).exists()) {
			LERROR_ << "Missing destination path " << QSTR(destinationLocalPath());
			throw new DestinationPathMissing();
		}

		return true;
	}

	ProcessorInterface::RunState Processor::run() {
		LPLUGIN_ << "Running: " << QSTR(name());

		if (allSourcesProcessed())
			return ProcessorInterface::Done;

		m_source = prepareNextSource();

		m_process->setWorkingDirectory(qApp->applicationDirPath());

		QStringList arguments = commandLineArgs(m_source);
		LPLUGIN_ << "Running backup command: " << QSTR(arguments.join(" "));
		m_process->start(m_settings->rsyncCommand(), arguments);

		return ProcessorInterface::Started;
	}

	void Processor::setSources(QStringList sources) {
		m_sources.clear();
		m_filters.clear();

		foreach(QString source, sources) {
			EPath path(source);
			switch (path.type()) {
				case EPath::Path:
					m_sources.append(path);
					break;
				case EPath::Include:
				case EPath::Exclude:
					m_filters.append(source);
					break;
				default:
					break;
			}
		}

		m_current_source_index = 0;
		writeFiltersFile();
	}

	QString Processor::linkDestination(EPath source) const {
		if (not m_last_destination_path.isValid())
			return QString();

		// FIXME connect last valid destination to real device according to id.
		//m_last_destination_path.attachToDevice(m_destination_path.deviceId());

		if (m_last_destination_path.unixPath().toLower() == destinationCygwinPath(source).toLower())
			return QString();

		return QString("--link-dest=%1").arg(m_last_destination_path.unixPath());
	}


	QStringList Processor::commandLineArgs(EPath source) const {
		QStringList arguments;

		arguments << CLI_SWITCHES
				  << QString("--include-from=%1").arg(filtersFile())
				  << QString("--bwlimit=%1").arg(m_settings->speedLimit());

		if (not m_settings->rsyncSwitches().trimmed().isEmpty())
			arguments.append(m_settings->rsyncSwitches().split(" ", QString::SkipEmptyParts));

		if (not m_settings->sshCommand().isEmpty())
			arguments << QString("--rsh=\"%1\"").arg(m_settings->sshCommand());

		if (m_settings->deleteOldFiles())
			arguments << "--delete";

		arguments << "--timeout=60"; // FIXME add settings dialog option

		if (dryRun())
			arguments << SILENT_SWITCHES;

		if (not linkDestination(source).isNull())
			arguments << linkDestination(source);

		arguments << source.unixPath()
				  << destinationCygwinPath(source);

		return arguments;
	}

	void Processor::abort() {
		// Skip to after last source so we won't continue processing remaining sources
		m_current_source_index = m_sources.count();

		// Notify in the log that the system halted due to user aborting
		setResultState(Result::Aborted);

		// Abort current process;
		if (m_process->state() == QProcess::Running)
			QProcess::startDetached("cli/kill.exe", QStringList() << "-t" << "rsync");

	}

	QString Processor::filtersFile() const {
		return FILTERS_FILE;
	}

	void Processor::loadConfig() {
		LPLUGIN_ << QSTR(name()) << ": Loading settings";
		m_destination_path = EPath(m_settings->path());
	}

	void Processor::saveConfig() {
		if (m_config_ui == NULL)
			return;

		LPLUGIN_ << QSTR(name()) << ": Saving settings";
		m_config_ui->saveConfig(m_settings);
	}

	QWidget* Processor::configUi() {
		if (m_config_ui == NULL)
			m_config_ui = new Local::ConfigDialog();

		m_config_ui->loadConfig(m_settings);
		return m_config_ui;
	}

	void Processor::writeFiltersFile() const {
		QFile filters_file(filtersFile());
		filters_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		QTextStream out(&filters_file);

		// NOTE: using \n here instead of qt::endl on purpose. newlines in include/exclude rules for
		// rsync on windows must use unix newlines.
		out << filters().join("\n");

	}

	QList<EPath> Processor::sources() const {
		return m_sources;
	}

	#define PATH_MACRO(name, value) e_path = e_path.replace(name, value, Qt::CaseInsensitive);

	QString Processor::expandPath(QString path, QDateTime datetime) const {
		QLocale locale(QLocale::English);
		QString e_path = path;
		PATH_MACRO("%WEEKDAY%",locale.dayName(datetime.date().dayOfWeek()));
		PATH_MACRO("%MONTHDAY%", QVariant(datetime.date().day()).toString());
		PATH_MACRO("%MONTHNAME%", locale.monthName(datetime.date().month()));
		PATH_MACRO("%HOSTNAME%", QHostInfo::localHostName());
		return e_path;
	}

	bool Processor::jobCleanup() {
		return true;
	}

}

Q_EXPORT_PLUGIN2(processor_local, Local::Processor)
