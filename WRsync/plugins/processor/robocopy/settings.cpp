#include "settings.h"

namespace RSync {

	Settings::Settings(QObject *parent):
		QSettings("rsync.ini", QSettings::IniFormat, parent) {
	}

	QString Settings::hostname() {
		return value("hostname", QString()).toString();
	}

	QString Settings::username() {
		return value("username", QString()).toString();
	}

	QString Settings::password() {
		return value("password", QString()).toString();
	}

	QString Settings::path() {
		return value("path", QString()).toString();
	}

	QString Settings::keyPath() {
		return value("key_path", QString()).toString();
	}

	void Settings::setHostname(QString hostname) {
		setValue("hostname", hostname);
	}

	void Settings::setUsername(QString username) {
		setValue("username", username);
	}

	void Settings::setPassword(QString password) {
		setValue("password", password);
	}

	void Settings::setPath(QString path) {
		setValue("path", path);
	}

	void Settings::setKeyPath(QString key_path) {
		setValue("key_path", key_path);
	}
}
