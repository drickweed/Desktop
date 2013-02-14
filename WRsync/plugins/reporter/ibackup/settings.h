#pragma once

/** QT headers **/
#include <QSettings>
#include <QUrl>

class Settings : public QSettings {
	Q_OBJECT
	public:
		Settings(QObject *parent=0);

		QString host() const;
		int port() const;
		QString user() const;
		QString password() const;

		QUrl url() const;
		QUrl baseUrl() const;

		void setHost(QString host);
		void setPort(int port);
		void setUser(QString user);
		void setPassword(QString password);

};
