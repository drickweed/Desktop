#include "installer.h"

#include <jb/std/log.h>

QMap<QProcess::ProcessError, QString> Installer::ErrorMessage;

Installer::Installer(QObject *parent):
	QObject(parent),
	busy(false),
	lastcode(-1),
	lastApp("none") {
	installer = new QProcess(this);
	connect(installer, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(cleanupFinished(int, QProcess::ExitStatus)));
	connect(installer, SIGNAL(error(QProcess::ProcessError)), SLOT(cleanupError(QProcess::ProcessError)));

	ErrorMessage[QProcess::FailedToStart] = "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.";
	ErrorMessage[QProcess::Crashed] = "The process crashed some time after starting successfully.";
	ErrorMessage[QProcess::Timedout] = "Installer process timed out.";
	ErrorMessage[QProcess::WriteError] = "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.";
	ErrorMessage[QProcess::ReadError] = "An error occurred when attempting to read from the process. For example, the process may not be running.";
	ErrorMessage[QProcess::UnknownError] = "An unknown error occurred.";
}

Installer::~Installer() {

}

void Installer::install(Version version, const bool dryrun) {

	if (busy)
		throw Busy();

	busy = true;
	run(version, dryrun);
}

void Installer::run(Version app, bool dryrun) {
	QString command;

	if (!app.hasInstaller()) {
		LOG ("No installer for: " << app.name());
		return;
	}

	// FIXME proper string escaping and protection.
	switch (app.type) {
		case Version::EXE:
			command = QString("\"%1\" %2").arg(app.installer().c_str()).arg(app.parameters().c_str());
			break;
		case Version::MSI:
			command = QString("msiexec /quiet /qb /i \"%1\" %2").arg(app.installer().c_str()).arg(app.parameters().c_str());
			break;
		case Version::NONE:
			break;
	}
	if (!dryrun) {
		LOG ("Running: " << command.toStdString());
		lastApp = app.name();
		installer->start(command);

	} else {
		LOG ("Pretending to run: " << command.toStdString());
	}

}

void Installer::commonCleanup() {
	// FIXME : option to dump stdout/stderr to (separate?) log files
	installer->readAllStandardOutput();
	installer->readAllStandardError();
	lastcode = installer->exitCode();
	busy = false;
}

void Installer::cleanupFinished(int /* result */, QProcess::ExitStatus /* status */) {
	commonCleanup();

	LOG ("Installation result code: " << lastcode);
	emit done();
}

void Installer::cleanupError(QProcess::ProcessError error) {
	commonCleanup();

	LOG ("Automate error code: " << ErrorMessage[error].toStdString());
	emit done();
}
