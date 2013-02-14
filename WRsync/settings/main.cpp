/** QT headers **/
#include <QApplication>
#include <QDir>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** Application headers **/
#include "mainwindow.h"
#include <wrsync/pluginmanager.h>
#include <wrsync/logfile.h>

using namespace JB;

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(wrsync);

	QApplication app(argc, argv);
	QDir::setCurrent(qApp->applicationDirPath());

	init_logs();

	LSEPARATOR_;
	LAPP_ << "Starting settings manager";

	PluginManager::loadPlugins();

	MainWindow *window = new MainWindow;
	window->show();

	int result =  app.exec();

	return result;

}

