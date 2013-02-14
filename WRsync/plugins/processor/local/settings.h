#pragma once

/** QT headers **/
#include <QSettings>
#include <QStringList>

namespace Local {

	class Settings : public QSettings {
		Q_OBJECT
		public:
			Settings(QObject *parent=0);

			QString path();
			QStringList ids();
			bool attachRemovable();
			int speedLimit();
			QString sshCommand();
			bool deleteOldFiles();
			QString rsyncCommand();
			QString rsyncSwitches();

		public slots:

			void setPath(QString path);
			void setIds(QStringList ids);
			void setAttachRemovable(bool attach);
			void setSpeedLimit(int speed);
			void setSshCommand(QString command);
			void setDeleteOldFiles(bool delete_old_files);
			void setRsyncCommand(QString rsync_command);
			void setRSyncSwitches(QString rsync_switches);
	};
}
