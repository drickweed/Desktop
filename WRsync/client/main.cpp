/** QT headers **/
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QTimer>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** STL headers **/
#include <iostream>
#include <string>

/** Application headers **/
#include "exitcodes.h"
#include "report.h"
#include "options.h"
#include "processor.h"
#include <wrsync/settings.h>
#include <plugins/interface/processor.h>
#include <wrsync/exceptions.h>
#include <wrsync/logfile.h>
#include <wrsync/shadowmanager.h>
#include <wrsync/sourcemanager.h>

using namespace std;
using namespace WRsync;

void savePID() {
	QFile pid_file("wrsync.pid");
	pid_file.open(QIODevice::WriteOnly);
	QTextStream pid_text(&pid_file);
	pid_text << qApp->applicationPid();
	pid_file.close();
}

int loadPID() {
	QFile pid_file("wrsync.pid");
	if (not pid_file.open(QIODevice::ReadOnly))
		return -1;
	QTextStream pid_text(&pid_file);
	QString pid = pid_text.readLine();
	return pid.toInt();
}

void resetRequestFlags() {
	QFile abort("temp/abort.sig");
	abort.remove();
}

int main(int argc, char *argv[]) {
	QCoreApplication *app = new QCoreApplication(argc, argv);
	QDir::setCurrent(app->applicationDirPath());

	init_logs();

	LSEPARATOR_;
	LAPP_ << "Starting wrsync";

	// If this is a console application, let the user know what is going on.
	cout << "Running backup. Closing this window cancels the backup." << endl;

	Result::ensureBaseLogPath();
	Processor::ensureBaseTempPath();
	PluginManager::loadPlugins();

	Settings settings;

	Options options(app->arguments());

	LAPP_ << "Loading processor";
	ProcessorInterface *processor_plugin = PluginManager::processor(settings.processorPlugin());

	processor_plugin->setDryRun(options.dry_run);

	LAPP_ << "Loaded processor: " << QSTR(processor_plugin->name());

	LAPP_ << "Initializing processor";
	Processor processor(processor_plugin);

//	LAPP_ << "Creating system snapshop shadow volumes";

//	ShadowManager shadow_manager;
//
//	SourceManager source_manager;
//	source_manager.setSources(settings.sources());
//
//	shadow_manager.setDrives(source_manager.localDrives());
//	shadow_manager.start();

	processor.processor()->setSources(settings.sources());

	LAPP_ << "Initializing reporter";
	Report reporter(&processor, &settings);

	processor.processor()->setReporterPlugin(reporter.firstReporter());

	if (options.dry_run) {
		QObject::connect(&processor, SIGNAL(done()), app, SLOT(quit()), Qt::QueuedConnection);
	} else {
		QObject::connect(&processor, SIGNAL(done()), &reporter, SLOT(reportUsingPlugins()), Qt::QueuedConnection);
		QObject::connect(&reporter, SIGNAL(done()), app, SLOT(quit()), Qt::QueuedConnection);
	}

	resetRequestFlags();
	QTimer requests;
	requests.setInterval(3000);
	QObject::connect(&requests, SIGNAL(timeout()), &processor, SLOT(processUserRequests()), Qt::QueuedConnection);
	requests.start();

	LAPP_ << "Starting backup processor";

	try {
		processor.start();
	// FIXME: Move this catch into the processor class
	} catch (ErrorOpeningLogFile *e){
		LERROR_ << "Error opening log file: " << e->what();
		exit(WRsync::ExitCode::ERROR_OPENING_LOG_FILE);
	}

	LDEBUG_ << "Starting QT event loop";

	int result = app->exec();

//	LAPP_ << "Removing system snapshop shadow volumes";

//	shadow_manager.stop();

	app->deleteLater();

	return result;
}
