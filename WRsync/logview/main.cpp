/** QT headers **/
#include <QApplication>
#include <QDir>
#include <QTranslator>
#include <QLibraryInfo>

/** Application headers **/
#include <wrsync/pluginmanager.h>
#include <wrsync/logfile.h>
#include "mainwindow.h"
#include "messages.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QDir::setCurrent(qApp->applicationDirPath());

	init_logs();
	LSEPARATOR_;
	LAPP_ << "Starting log viewer";

	QTranslator translator;
	translator.load("logview_" + QLocale::system().name(),
	QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&translator);

	PluginManager::loadPlugins();
	Messages::init_messages();

	MainWindow window;
	window.show();

	return app.exec();
}
