#include "settings.h"

#include <QCoreApplication>

Settings::Settings() {
}

QString Settings::dataFolder() {
	return qApp->applicationDirPath() + "/../data/";
}
