#include "epath.h"

/** QT headers **/
#include <QDir>
#include <QProcess>
#include <QSet>

/** JB headers **/
#include <jb/qt4/strings.h>

/** Application headers **/
#include <wrsync/logfile.h>
#include "exceptions.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#define sleep(n) Sleep(1000 * n)
#endif

using namespace JB;

EPath::EPath() {
	m_remote_path_mounted = false;
}

EPath::EPath(QString path) {
	setPath(path);
	m_remote_path_mounted = false;
}

QString EPath::removableRelativePath() const {
	QString path = m_path.mid(REMOVABLE_URI.length());
	return path.mid(path.indexOf('/'));
}

QString EPath::removableId() const {
	QString path = m_path.mid(REMOVABLE_URI.length());
	return path.left(path.indexOf('/'));
}

bool EPath::isValid(QString path) {
#ifdef MIMIC_WINDOWS
	return true;
#else
	return (not path.isEmpty() and
				(
				  ((QDir(path).isAbsolute()) and (QDir(path).exists())) or
				  (EPath(path).isNetworkPath()) or
				  (EPath(path).isRemovable()) or
				  (EPath(path).isRSyncPath())
				)
			);
#endif
}

QString EPath::cygwinPath(QChar drive_letter) const {
	QString pathSegments = path().mid(3, path().size()-3);
	pathSegments = QDir::fromNativeSeparators(pathSegments);

	return QString("/cygdrive/%1/%2").arg(drive_letter).arg(pathSegments);
}

QList<QChar> EPath::existingDriveLetters() {
	QList<QChar> letters;
	foreach(QFileInfo info, QDir::drives())
		letters << info.absolutePath()[0].toLower();
	return letters;
}

QList<QChar> EPath::availableDriveLetters() {
	QSet<QChar> letters;
	QString alpha = "abcdefghijklmnopqrstuvwxyz"; // FIXME: find a better way to get the latin alphabet
	for (int i=0; i < alpha.size(); i++)
		letters << alpha[i];
	return letters.subtract(existingDriveLetters().toSet()).toList();
}

bool EPath::prepareDatabaseFiles() {
	QProcess database_dump;
	QString dump_command = "cli\\mssqldump";
	QStringList dump_command_arguments = QStringList() << "-A" << "-f" << "accounting-database";
	database_dump.start(dump_command, dump_command_arguments);
	database_dump.waitForFinished();
	return (database_dump.exitCode() == 0) && (database_dump.exitStatus() == QProcess::NormalExit);
}

bool EPath::mountNetwork(QChar drive) {
	// Mount network drive
	QProcess net_use;
	LAPP_ << "Mounting network path to " << drive.toAscii();
	QString mount_command = QString("net use %1: \"%2\"").arg(drive).arg(m_path);
	LDEBUG_ << "Executing: " << QSTR(mount_command);
	net_use.start(mount_command);
	net_use.waitForFinished();
	if (net_use.exitCode() != 0)
		return false;

	m_remote_path_mounted = true;

	m_usable_drive = drive;

	return true;
}

bool EPath::unmountNetwork() {

	if (not m_remote_path_mounted)
		return true;

	QString umount_command = QString("net use %1: /delete").arg(m_usable_drive);
	LDEBUG_ << "Running umount command: " << QSTR(umount_command);

	QProcess net_use;
	net_use.start(umount_command);
	net_use.waitForFinished();
	if (net_use.exitCode() != 0)
		return false;

	m_remote_path_mounted = false;
	m_usable_drive = QChar::Null;
	return true;
}

QString EPath::path() const {
	QString usable_path;

	if (isMsSql()) {
		usable_path = QString();
	} else if (isRemovable()) {
		// Path to removable device
		if (m_usable_drive.isNull())
			throw new ErrorLocatingRemovableDevice;
		usable_path = QString("%1:\%2").arg(m_usable_drive, removableRelativePath());
	} else if (m_usable_drive.isNull()) {
		// Got a useable path
		usable_path = m_path;
	} else if (m_remote_path_mounted) {
		// Got a remote path
		usable_path = QString("%1:").arg(m_usable_drive);
	} else {
		// Got the local path with a different drive letter
		usable_path = QString("%1%2").arg(m_usable_drive).arg(m_path.mid(1));
	}

	return usable_path;
}

QString EPath::unixPath() const {
	if (isRSyncPath()) {
		return m_path.mid(4);
	}

#ifdef Q_OS_WIN32
	return cygwinPath(path().at(0)).toLower();
#else
	return path();
#endif
}

QString EPath::shadowPath() const {
#ifdef Q_OS_WIN32
	if (m_shadow_drive.isNull())
		return unixPath();

	return cygwinPath(m_shadow_drive);

#else
	return unixPath();
#endif

}

void EPath::mountShadow() {

}

void EPath::umountShadow() {

}

QString EPath::realPath() const {
#ifdef Q_OS_WIN32
	return m_path.toLower();
#else
	return m_path;
#endif
}

bool EPath::attachToDevice(QStringList device_ids) {
	QChar found_drive;
	foreach(QString id, device_ids) {
		found_drive = EPath::driveLetterById(id);
		if (found_drive.isNull())
			continue;
		break;
	}
	setDriveLetter(found_drive);
	return m_usable_drive != QChar::Null;
}

QChar EPath::driveLetter() const {
	if (not m_usable_drive.isNull())
		return m_usable_drive;

	return m_path.at(0);
}

bool EPath::operator== (EPath &other) const {
	return (realPath() == other.realPath()) and
			isLocalPath() and
			(lastDeviceId() == other.driveID() or driveID() == other.lastDeviceId());
}

QString EPath::driveID(QChar drive_letter) {
	if (drive_letter.isNull())
		return QString();

	QString command_line = QString("cli\\USBToolChest -u -d %1:").arg(drive_letter);
	LDEBUG_ << "Running: " << QSTR(command_line);

	QProcess process;
	process.start(command_line);
	process.waitForFinished();
	QString id = process.readAll();

	if (process.exitCode() == 0)
		return id.trimmed();

	return QString();
}

QString EPath::driveID() const {
	return driveID(driveLetter());
}

QChar EPath::driveLetterById(QString id) {
	if (id.isEmpty())
		return QChar::Null;

	QString command_line = QString("cli\\USBToolChest -t -d %1").arg(id);
	LDEBUG_ << "Running: " << QSTR(command_line);

	QProcess process;
	process.start(command_line);
	process.waitForFinished();
	QString drive = process.readAll();

	if (process.exitCode() == 0)
		return drive[0];
	else
		return QChar::Null;
}

void EPath::setDriveByID(QString id) {
	m_usable_drive = driveLetterById(id);
}

bool EPath::matchesDiskID(QString id) const {
	return driveLetter() == driveLetterById(id);
}

bool EPath::matchesDiskID(QStringList ids) const {
	foreach(QString id, ids)
		if (matchesDiskID(id))
			return true;
	return false;
}
