#include "settings.h"

Settings::Settings(QObject *parent) :
		QSettings("ibackup.ini", QSettings::IniFormat, parent) {
	sync();
}

QString Settings::host() const {
	return value("host", QString()).toString();
}

QString Settings::user() const {
	return value("user", QString()).toString();
}

QString Settings::password() const {
	return value("password", QString()).toString();
}

void Settings::setHost(QString host) {
	setValue("host", host);
}

void Settings::setUser(QString user) {
	setValue("user", user);
}

void Settings::setPassword(QString password) {
	setValue("password", password);
}

int Settings::port() const {
	return value("port", 3002).toInt();
}

void Settings::setPort(int port) {
	setValue("port", port);
}

QUrl Settings::url() const {
	QUrl url = baseUrl();
	url.setPath("/backup_reports.xml");
	return url;
}

QUrl Settings::baseUrl() const {
	QUrl url;
	url.setScheme("http");
	url.setHost(host());
	url.setPort(port());
	return url;
}
