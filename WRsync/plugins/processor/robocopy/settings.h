#pragma once

/** QT headers **/
#include <QSettings>

namespace RSync {

	class Settings : public QSettings {
		Q_OBJECT
		public:
			Settings(QObject *parent=0);

			QString hostname();
			QString username();
			QString password();
			QString path();
			QString keyPath();

		public slots:

			void setHostname(QString hostname);
			void setUsername(QString username);
			void setPassword(QString password);
			void setPath(QString path);
			void setKeyPath(QString keyPath);

	};
}
