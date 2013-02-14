#pragma once

/** QT headers **/
#include <QObject>

class Monitor : public QObject {
	Q_OBJECT
	public:
		Monitor(QObject *parent=0);

		bool backupRunning() const;

	public slots:
		void stopBackup() const;

	signals:

	public slots:

};
