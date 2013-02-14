#include "settings.h"

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** QT headers **/
#include <QApplication>
#include <QDesktopServices>

using namespace JB;

Settings::Settings(QObject *parent):
	QObject(parent),
	settings(NULL) {
	loadSettings();
}

QString Settings::collectionsPath() const {
    QString documents_path = Path::join(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), getAppName());
    return settings->value("collections_path", QVariant(Path::join(documents_path, "processed"))).toString();
}

void Settings::setCollectionsPath(QString path) {
	settings->setValue("collections_path", path);
}

QString Settings::ftpHost() const {
    return settings->value("ftp_host", QVariant("")).toString();
}

void Settings::setFtpHost(QString host) {
	settings->setValue("ftp_host", host);
}

QString Settings::ftpUser() const {
    return settings->value("ftp_user", QVariant("")).toString();
}

void Settings::setFtpUser(QString user) {
	settings->setValue("ftp_user", user);
}

QString Settings::ftpPassword() const {
    return settings->value("ftp_password", QVariant("")).toString();
}

void Settings::setFtpPassword(QString password) {
	settings->setValue("ftp_password", password);
}

void Settings::validateSettings() {
	Path::ensurePath(collectionsPath());
}

Settings::~Settings() {
	sync();
	delete settings;
}

void Settings::loadSettings() {
	if (settings) {
		delete settings;
		settings = NULL;
	}
    QString data_path = Path::join(QDesktopServices::storageLocation(QDesktopServices::DataLocation), getAppName());
    settings = new QSettings(Path::join(data_path, "imageprep.ini"), QSettings::IniFormat);
	validateSettings();
}
