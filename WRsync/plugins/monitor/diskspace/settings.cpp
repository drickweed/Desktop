#include "settings.h"

namespace DiskSpace {

	Settings::Settings(QObject *parent):
		QSettings("local.ini", QSettings::IniFormat, parent) {
	}

	QString Settings::path() {
		return value("path", QString()).toString();
	}

	void Settings::setPath(QString path) {
		setValue("path", path);
	}

}
