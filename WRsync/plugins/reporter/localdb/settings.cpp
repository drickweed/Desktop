#include "settings.h"

Settings::Settings(QObject *parent) :
		QSettings("localdb.ini", QSettings::IniFormat, parent) {
	}

	QString Settings::path() const {
		return value("path", "wrsync.sqlite3").toString();
	}

	void Settings::setPath(QString path) {
		setValue("path", path);
	}
