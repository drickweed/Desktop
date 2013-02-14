#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(imageprep);

	QApplication app(argc, argv);

	MainWindow window;

	if (app.arguments().count() > 1)
		window.loadImages(app.arguments()[1]);

	window.show();
	return app.exec();
}
