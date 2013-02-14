/** QT headers **/
#include <QApplication>
#include <QCleanlooksStyle>

/** Application headers **/
#include "mainwindow.h"
#include <settings/system.h>

/** JB headers **/
#include <jb/std/log.h>
#include <jb/qt4/app.h>

/** STL headers **/
#include <exception>

using namespace ImageImp;
using namespace JB;

int main(int argc, char **argv) {

	Q_INIT_RESOURCE(config);

	App::init(argc, argv);

	QApplication app(argc, argv);

	/** Logging **/
	Logger::init("imageimp.log");
	Config config;
	Logger::Instance().setEnabled(false);

	/** System init **/
	System::init();

	// Style
	app.setStyle(new QCleanlooksStyle);

	// Main window
	MainWindow *window = new MainWindow();

	// Show main window
	window->show();

	return app.exec();
}
