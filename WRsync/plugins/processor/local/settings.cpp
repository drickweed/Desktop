#include "settings.h"

namespace Local {

	Settings::Settings(QObject *parent):
		QSettings("local.ini", QSettings::IniFormat, parent) {
	}

	QString Settings::path() {
		return value("path", QString()).toString();
	}

	void Settings::setPath(QString path) {
		setValue("path", path);
	}

	QStringList Settings::ids() {
		return value("valid_device_ids", QStringList()).toStringList();
	}

	void Settings::setIds(QStringList ids) {
		setValue("valid_device_ids", ids);
	}

	bool Settings::attachRemovable() {
		return value("attach_removable", false).toBool();
	}

	void Settings::setAttachRemovable(bool attach) {
		setValue("attach_removable", attach);
	}

	int Settings::speedLimit() {
		return value("speed_limit", 0).toInt();
	}

	void Settings::setSpeedLimit(int speed) {
		setValue("speed_limit", speed);
	}

	QString Settings::sshCommand() {
		return value("ssh_command", QString()).toString();
	}

	void Settings::setSshCommand(QString command) {
		setValue("ssh_command", command);
	}

	bool Settings::deleteOldFiles() {
		return value("delete_old_files", true).toBool();
	}

	void Settings::setDeleteOldFiles(bool delete_old_files) {
		setValue("delete_old_files", delete_old_files);
	}

	QString Settings::rsyncCommand() {
		QString command;
		command = value("rsync_command", "cygwin\\rsync").toString();
		if (command.isEmpty())
			command = "cygwin\\rsync";
		return command;
	}

	void Settings::setRsyncCommand(QString rsync_command) {
		setValue("rsync_command", rsync_command);
	}

	QString Settings::rsyncSwitches() {
		return value("rsync_switches", QString()).toString();
	}

	void Settings::setRSyncSwitches(QString rsync_switches) {
		setValue("rsync_switches", rsync_switches);
	}
}
