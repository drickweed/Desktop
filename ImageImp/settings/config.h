#pragma once

/** QT headers **/
#include <QObject>
#include <QString>
#include <QSettings>

namespace ImageImp {

	class Config : public QObject {
		Q_OBJECT
		public:
			Config();
			~Config();

			QString language() const;
			void setLanguage(QString language);

			bool logging() const;
			void setLogging(bool logging);

			QSettings *settings;

			void sync() { settings->sync(); }
	};

}
