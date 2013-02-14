/** QT headers **/
#include <QtGui/QApplication>
#include <QDir>

/** Application headers **/
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(wrsync);
\
	QApplication app(argc, argv);
	QDir::setCurrent(app.applicationDirPath());

	MainWindow window;

	return app.exec();
}
