
/** QT headers **/
#include <QtCore/QCoreApplication>
#include <QDir>

/** Application headers **/
#include <wrsync/settings.h>

/** JB headers **/
#include <jb/std/log.h>
#include <jb/qt4/pathutils.h>

/** STL headers **/
#include <iostream>

using namespace WRsync;
using namespace std;

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QDir::setCurrent(qApp->applicationDirPath());


	return 0;
}
