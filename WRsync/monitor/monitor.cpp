#include "monitor.h"

/** QT headers **/
#include <QProcess>

/** JB headers **/
#include <jb/qt4/fileutils.h>

using namespace JB;

Monitor::Monitor(QObject *parent) :
	QObject(parent) {
}

bool Monitor::backupRunning() const {
	QProcess process;
	process.start("cli/Process -r wrsync");
	process.waitForFinished();
	if (process.exitCode() == 0)
		return true;

	process.start("cli/Process -r wrsync-console");
	process.waitForFinished();
	if (process.exitCode() == 0)
		return true;

	return false;
}

void Monitor::stopBackup() const {
	File::touch("temp/abort.sig");
}
