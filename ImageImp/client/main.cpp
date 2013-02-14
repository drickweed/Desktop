#define _BIND_TO_CURRENT_VCLIBS_VERSION 1;

/** QT headers **/
#include <QApplication>
#include <QPlastiqueStyle>
#include <QCleanlooksStyle>
#include <QTranslator>
#include <QFile>

/** Application headers **/
#include "widgets/mainwindow.h"
#include "document/document.h"
#include "settings/system.h"
#include "settings/startup.h"
#include "settings/config.h"

/** JB headers **/
#include <jb/database/database.h>
#include <jb/qt4/styler.h>
#include <jb/qt4/app.h>
#include <jb/qt4/pathutils.h>
#include <jb/std/log.h>

/** STL headers **/
#include <exception>

using namespace ImageImp;
using namespace JB;

int main(int argc, char **argv) {

	App::init(argc, argv);

	QCoreApplication::addLibraryPath(".");

	QApplication app(argc, argv);

	/** Logging **/
	Logger::init("imageimp.log");
	Config config;
	Logger::Instance().setEnabled(config.logging());

	/** Resources **/
	QString resource_path = Path::join(App::path().c_str(), "imageimp.rcc");

	if (not QFile(resource_path).exists()) {
		QMessageBox::critical(NULL, "Missing resource file", "imageimp.rcc resource file missing");
		return 0;
	}

	QResource::registerResource(resource_path);

	/** System init **/
	System::init();

	/** Language **/
	QTranslator translator;
	if (not translator.load(System::translationFile())) {
		LOG("Failed to load translations");
	}
	app.installTranslator(&translator);

	if (System::rightToLeftLanguage())
		app.setLayoutDirection(Qt::RightToLeft);

	/** Startup **/
	try {
		ensureFunctionalState();
	} catch (std::exception *e) {
		qDebug() << e->what();
		return 10;
	}

	Database::init(System::databasePath(), ":/scripts/imageimp.sql");

	// Style
	app.setStyle(new QCleanlooksStyle);

	// Document
	Document document;

	// Main window
	MainWindow *window = new MainWindow(&document);
	window->resize(780,550);

	// Style fixes
	setEmptyLayoutSpacing(window);

	// Show main window
	window->show();

	// Style reload button. Uncomment when editing the stylesheet
// 	Styler styler;
// 	styler.setStylesheetFile(":/styles/imageimp.css");
// 	styler.reloadStylesheet();
// 	styler.show();

	return app.exec();
}
