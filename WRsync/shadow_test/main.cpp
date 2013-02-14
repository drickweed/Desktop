#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QDebug>

#include <wrsync/shadowmanager.h>
#include <wrsync/sourcemanager.h>

using namespace WRsync;

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
	ShadowVolume shadow_manager;

	QStringList sources;
	sources.append("C:\\windows");

	SourceManager source_manager;
	source_manager.setSources(sources);

	qDebug() << source_manager.localDrives();

	shadow_manager.setDrive(source_manager.localDrives()[0]);
	shadow_manager.start();

	qDebug() << shadow_manager.shadowDrive();

	//shadow_manager.stop();

	return a.exec();
}
