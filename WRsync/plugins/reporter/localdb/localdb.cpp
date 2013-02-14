#include "localdb.h"

/** QT headers **/
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtPlugin>
#include <QFile>
#include <QDebug>

/** JB headers **/
#include <jb/database/database.h>

/** Application headers **/
#include "report.h"
#include "wrsync/logfile.h"

using namespace JB;

LocalDBReporter::LocalDBReporter():
	QObject(),
	m_config_ui(NULL) {

	Q_INIT_RESOURCE(localdb);
	initDatabase();
}

void LocalDBReporter::initDatabase() {
	Database::init(databasePath(), ":/database/localdb.sql");
}

void LocalDBReporter::resetDatabase() {
	QFile db(databasePath());
	if (db.exists())
		db.remove();

	initDatabase();
}

LocalDBReporter::~LocalDBReporter() {
	saveConfig();
}

QWidget *LocalDBReporter::configUi() {
	if (not m_config_ui) {
		m_config_ui = new LocalDb::ConfigDialog();
		m_config_ui->loadSettings(&settings);
		connect(m_config_ui, SIGNAL(resetDatabase()), SLOT(resetDatabase()));
	}

	return m_config_ui;
}

void LocalDBReporter::saveConfig() {
	if (m_config_ui == NULL)
		return;

	LPLUGIN_ << QSTR(name()) << ": Saving settings";
	m_config_ui->saveSettings(&settings);
}

void LocalDBReporter::loadConfig() {
	m_path = settings.path();
}

bool LocalDBReporter::report(Result result) {
	Report report;
	report.fromResult(result);
	return report.save();
}

QMap<QDateTime, int> LocalDBReporter::resultsForMonth(int month, int year) const {

	QMap<QDateTime, int> results;
	QString dateSearch = QString("%1-%2-%").arg(year).arg(month, 2, 10, QLatin1Char('0'));

	Report find_report;
	QList<RelationalObjectRef> objects = find_report.find("started_at", dateSearch);

	foreach (RelationalObjectRef object, objects) {
		Report report(object);
		results[report.startedAt()] |= report.state();
	}

	return results;
}

QList<Result> LocalDBReporter::resultsForDay(QDate day) const {
	QList<Result> results;
	QString dateSearch = day.toString("yyyy-MM-ddT%");

	LDEBUG_ << "search date: " << QSTR(dateSearch);

	Report find_report;
	QList<RelationalObjectRef> objects = find_report.find("started_at", dateSearch);

	LDEBUG_ << "Result count: " << objects.count();

	foreach (RelationalObjectRef object, objects) {
		Report report(object);
		results.append(report.toResult());
	}

	LDEBUG_ << "Result count: " << results.count();
	return results;
}

EPath LocalDBReporter::findLastValidForSource(QString source, QString destination_id) const {

	if (source.isEmpty())
		return EPath();

	LPLUGIN_ << "Looking for valid previous backup for " << QSTR(source);

	QSqlQuery query;

	if (destination_id.isEmpty())
		query.prepare("select * from reports where source = ? and destination_id IS NULL order by started_at desc limit 1");
	else {
		query.prepare("select * from reports where source = ? and destination_id = ? order by started_at desc limit 1");
		query.bindValue(1, destination_id);
	}

	query.bindValue(0, source);
	query.exec();

	EPath last_valid_backup;

	while (query.next()) {
		Report report(query.record());
		LPLUGIN_ << "Looking in " << QSTR(report.destinationPath());
		last_valid_backup.setPath(report.destinationPath());
		last_valid_backup.setLastDeviceId(report.destinationId());
		if (last_valid_backup.isValid()) {
			return last_valid_backup;
		}
	}

	return EPath();
}

Q_EXPORT_PLUGIN2(reporter_localdb, LocalDBReporter)
