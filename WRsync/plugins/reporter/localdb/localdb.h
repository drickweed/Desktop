#pragma once

/** QT headers **/
#include <QWidget>
#include <QString>
#include <QSettings>

/** Application headers **/
#include <interface/reporter.h>
#include "localdbconfigdialog.h"
#include "settings.h"

using namespace LocalDb;

class LocalDBReporter: public QObject, public ReporterInterface {
	Q_OBJECT
	Q_INTERFACES(ReporterInterface)

	public:
		LocalDBReporter();

		~LocalDBReporter();

		QString name() const { return "Local database"; }

		QString id() const { return "d77a3690-1b9a-11df-ac94-0016d3298e6d"; }

		QWidget *configUi();

		void saveConfig();

		void loadConfig();

		bool report(Result result);

		QMap<QDateTime, int> resultsForMonth(int month, int year=QDate::currentDate().year()) const;

		QList<Result> resultsForDay(QDate day) const;

		EPath findLastValidForSource(QString source, QString destination_id) const;

	private slots:
		void resetDatabase();

	private:
		void initDatabase();

		QString databasePath() const {
			return settings.path();
		}

		ConfigDialog *m_config_ui;

		QString m_path;
		Settings settings;

		void ensureReportTable();
};
