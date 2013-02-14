#pragma once

/** QT headers **/
#include <QSettings>

class Settings : public QSettings {
	Q_OBJECT
	public:
		Settings(QObject *parent=0);

		QString path() const;

	public slots:

		void setPath(QString path);

};

