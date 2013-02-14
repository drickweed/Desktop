#pragma once

/** QT headers **/
#include <QObject>
#include <QString>
#include <QSettings>

class Settings : public QObject {
	public:
		Settings(QObject *parent=0);
		~Settings();

		QString collectionsPath() const;
		void setCollectionsPath(QString path);

		QString ftpHost() const;
		void setFtpHost(QString host);

		QString ftpUser() const;
		void setFtpUser(QString user);

		QString ftpPassword() const;
		void setFtpPassword(QString password);

		void validateSettings();

		void sync() { settings->sync(); }
		void reload() { loadSettings(); }

	private:
		QSettings *settings;

		void loadSettings();
        QString getAppName() const { return "ImagePrep"; }

};
