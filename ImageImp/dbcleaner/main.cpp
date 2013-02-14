/** QT headers **/
#include <QApplication>
#include <QPlastiqueStyle>
#include <QCleanlooksStyle>
#include <QFileInfo>
#include <QString>

/** Application headers **/
#include "mainwindow.h"
#include "processor/clean_db.h"
#include "settings/system.h"

/** JB headers **/
#include <jb/database/database.h>
#include <jb/qt4/app.h>
#include <jb/qt4/pathutils.h>

/** STL headers **/
#include <exception>

using namespace ImageImp;
using namespace JB;

int main(int argc, char **argv) {

	QApplication app(argc, argv);
	App::init(argc, argv);
	System::init();

	QString database_path = System::databasePath();

	if (not QFileInfo(database_path).exists())
		return 0;

	Database::add(database_path);
	DatabaseCleaner().clean();

	// Style
	app.setStyle(new QCleanlooksStyle);

	// Main window
	MainWindow *window = new MainWindow();

	QObject::connect(window->closeButton, SIGNAL(clicked()), &app, SLOT(quit()));

	// Show main window
	window->show();

	return app.exec();
}
