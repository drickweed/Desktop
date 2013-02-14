#pragma once

/** QT headers **/
#include <QWidget>
#include <QHttp>

/** Application headers **/
#include <interface/reporter.h>

#include "ibackupconfigdialog.h"
#include "settings.h"

class IBackupReporter: public QObject, public ReporterInterface {
	Q_OBJECT
	Q_INTERFACES(ReporterInterface)

	public:
		IBackupReporter();
		~IBackupReporter();

		QWidget *configUi();

		QString name() const { return "IBackup"; }

		QString id() const { return "d04d27ba-1b9a-11df-ae99-0016d3298e6d"; }

		void saveConfig();

		void loadConfig();

		bool report(Result result);

		QMap<QDateTime, int> resultsForMonth(int month, int year=QDate::currentDate().year()) const;

		QList<Result> resultsForDay(QDate date) const;

		EPath findLastValidForSource(QString source, QString destination_id) const;

	private:
		IBackupConfigDialog *m_config_ui;
		Settings m_settings;
		QHttp *reportRequest;
};
