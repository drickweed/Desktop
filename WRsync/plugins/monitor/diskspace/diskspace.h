#pragma once

/** QT headers **/
#include <QObject>
#include <QProcess>
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDir>

/** Application headers **/
#include <wrsync/epath.h>
#include <wrsync/result.h>
#include <interface/monitor.h>
#include "configdialog.h"
#include "settings.h"
#include <wrsync/logfile.h>

namespace DiskSpace {

	class Monitor : public QObject, public MonitorInterface {
		Q_OBJECT
		Q_INTERFACES(MonitorInterface)

		public:
			Monitor();
			~Monitor();

			QString name() const { return "DiskSpace"; }
			QString id() const { return "7310d606-2f99-11df-bf3f-0016d5298e6e"; }

			void saveConfig();
			void loadConfig();

			QWidget *configUi();

			QString message() const { return ""; }
			int level() const { return 0; }

		private:

			DiskSpace::Settings *m_settings;

			DiskSpace::ConfigDialog *m_config_ui;

	};
}
