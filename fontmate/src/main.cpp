/** QT headers **/
#include <QApplication>
#include <QPlastiqueStyle>
#include <QCleanlooksStyle>

/** Application headers **/
#include "mainwindow.h"
#include <document/document.h>
#include <settings/settings.h>

/** JB headers **/
#include <jb/std/log.h>
#include <jb/models/tags.h>
#include <jb/database/database.h>
#include <jb/qt4/pathutils.h>

using namespace JB;
using namespace FontMate;

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(fontmate);

	QApplication app(argc, argv);

	Logger::init(Path::join(app.applicationDirPath(), "fontmate.log").toStdString());
	Logger::Instance().setEnabled(Settings().logging());

	Database::init(Path::join(app.applicationDirPath(), "fontmate.sqlite3"));

	Tags::initTagEngine();

	Document *document = new Document;

	// Style
#ifdef Q_OS_WIN32
        app.setStyle(new QCleanlooksStyle);
#endif

	MainWindow * mw = new MainWindow(document);

	mw->show();
	int result = app.exec();

	return result;
}
