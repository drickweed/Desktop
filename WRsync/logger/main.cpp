/** QT headers **/
#include <QtCore/QCoreApplication>
#include <QDir>

/** Application headers **/
#include "report.h"
#include <wrsync/settings.h>
#include <wrsync/result.h>
#include <plugins/interface/processor.h>

/** JB headers **/
#include <jb/std/log.h>
#include <jb/qt4/pathutils.h>

/** STL headers **/
#include <iostream>

using namespace WRsync;
using namespace std;

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QDir::setCurrent(qApp->applicationDirPath());

	if (app.arguments().count() < 2) {
		cout << "Syntax: <source> <exit_code>" << endl;
		return 1;
	}

	JB::Logger::init(JB::Path::join(app.applicationDirPath(), "wrsync.log").toStdString());
	LOGGER.setEnabled(true);

	Result::ensureBaseLogPath();

	PluginManager::loadPlugins();

	Settings settings;

	LOG("Initializing reporter");
	Report reporter(&settings);

	QString source_name = app.arguments()[1];
	int exit_code = QVariant(app.arguments()[2]).toInt();

	Result *result = new Result(EPath(source_name));
	result->setStartedAt(QDateTime::currentDateTime());
	result->setExitCode(exit_code);
	if (exit_code != 0)
		result->setState(Result::Error);
	else
		result->setState(Result::OK);
	result->setDestination("Manual logging record");

	reporter.reportUsingPlugins(result);

	// app.processEvents();
	return 0;
}
