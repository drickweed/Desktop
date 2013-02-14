/** QT headers **/
#include <QApplication>
#include <QDir>

/** Application headers **/
#include "notifier.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QDir::setCurrent(qApp->applicationDirPath());

	Notifier window;
	window.show();

	return app.exec();
}
