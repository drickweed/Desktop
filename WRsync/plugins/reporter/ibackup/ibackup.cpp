#include "ibackup.h"

/** QT headers **/
#include <QtPlugin>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QHttpRequestHeader>

/** Application headers **/
#include <wrsync/logfile.h>

IBackupReporter::IBackupReporter():
	QObject(),
	m_config_ui(NULL) {
}

IBackupReporter::~IBackupReporter() {
	saveConfig();
	delete reportRequest;
}

QWidget *IBackupReporter::configUi() {
	if (not m_config_ui) {
		m_config_ui = new IBackupConfigDialog();
		m_config_ui->loadSettings(&m_settings);
	}

	return m_config_ui;
}

void IBackupReporter::saveConfig() {
	if (m_config_ui == NULL)
		return;

	LPLUGIN_ << QSTR(name()) << ": Saving settings";
	m_config_ui->saveSettings(&m_settings);
}

void IBackupReporter::loadConfig() {

}

bool IBackupReporter::report(Result result) {

	reportRequest = new QHttp();
	QUrl url = m_settings.url();

	reportRequest->setHost(url.host(), QHttp::ConnectionModeHttp, url.port());
	QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");

	QByteArray xml = result.toXml();

	reportRequest->post(path, xml, 0);
	reportRequest->deleteLater();

	return true;
}

QMap<QDateTime, int> IBackupReporter::resultsForMonth(int month, int year) const {
	return QMap<QDateTime, int>();
}

QList<Result> IBackupReporter::resultsForDay(QDate date) const { return QList<Result>(); }

EPath IBackupReporter::findLastValidForSource(QString source, QString destination_id) const { return EPath(); }

Q_EXPORT_PLUGIN2(reporter_ibackup, IBackupReporter)
