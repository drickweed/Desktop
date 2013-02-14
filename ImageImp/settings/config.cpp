#include "config.h"

/** QT headers **/
#include <jb/qt4/app.h>
#include <jb/qt4/pathutils.h>

using namespace JB;

QString ImageImp::Config::language() const {
	return settings->value("language", QVariant("en")).toString();
}

ImageImp::Config::Config() {
	settings = new QSettings(Path::join(QString(App::path().c_str()), "imageimp.ini"), QSettings::IniFormat);
}

void ImageImp::Config::setLanguage(QString language) {
	settings->setValue("language", language);
}

ImageImp::Config::~ Config() {
	delete settings;
}

bool ImageImp::Config::logging() const {
	return settings->value("logging", QVariant(false)).toBool();
}

void ImageImp::Config::setLogging(bool logging) {
	settings->setValue("logging", logging);
}
