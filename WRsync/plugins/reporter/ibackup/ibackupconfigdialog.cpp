#include "ibackupconfigdialog.h"

/** QT headers **/
#include <QHttp>

/** Application headers **/
#include <wrsync/logfile.h>

IBackupConfigDialog::IBackupConfigDialog(QWidget *parent) :
	QWidget(parent){
	setupUi(this);
	connect(validateButton, SIGNAL(clicked()), SLOT(verifyConnection()));

	validationRequest = new QHttp(this);
	connect(validationRequest, SIGNAL(done(bool)), SLOT(processResult(bool)));
}

void IBackupConfigDialog::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

void IBackupConfigDialog::verifyConnection() {
	QUrl url;
	url.setScheme("http");
	url.setHost(hostnameLineEdit->text());
	url.setPort(portLineEdit->text().toInt());

	validationRequest->setHost(url.host(), QHttp::ConnectionModeHttp, url.port());
	QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");

	LDEBUG_ << "Validating server at " << QSTR(validationRequest->currentRequest().toString());

	validationRequest->get("/", NULL);
}

void IBackupConfigDialog::processResult(bool error) {
	if (error)
		validationResultLabel->setText("Failed");
	else
		validationResultLabel->setText("Success");
}

void IBackupConfigDialog::loadSettings(Settings *settings) {
	hostnameLineEdit->setText(settings->host());
	portLineEdit->setText(QString("%1").arg(settings->port()));
	usernameLineEdit->setText(settings->user());
	passwordLineEdit->setText(settings->password());
}

void IBackupConfigDialog::saveSettings(Settings *settings) {
	settings->setHost(hostnameLineEdit->text());
	settings->setPort(portLineEdit->text().toInt());
	settings->setUser(usernameLineEdit->text());
	settings->setPassword(passwordLineEdit->text());
	settings->sync();
}
