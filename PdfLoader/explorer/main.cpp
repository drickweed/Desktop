/** QT headers **/
#include <QtGui/QApplication>

/** Application headers **/
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	MainWindow window;

	if (argc > 1)
		window.loadFile(argv[1]);

	window.show();
	return app.exec();
}
