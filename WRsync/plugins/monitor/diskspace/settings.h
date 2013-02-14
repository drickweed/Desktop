#pragma once

/** QT headers **/
#include <QSettings>
#include <QStringList>

namespace DiskSpace {

	class Settings : public QSettings {
		Q_OBJECT
		public:
			Settings(QObject *parent=0);

			QString path();

		public slots:

			void setPath(QString path);


	};
}
