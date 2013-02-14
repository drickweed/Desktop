#pragma once

#include <QObject>
#include <QString>

class Settings : public QObject {
	Q_OBJECT

	public:
		Settings();

		QString dataFolder();
};

