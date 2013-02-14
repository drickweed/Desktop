#include "settingsdialog.h"

/** QT headers **/
#include <QFtp>
#include <QDebug>

SettingsDialog::SettingsDialog(Settings *settings, QWidget *parent) :
	QDialog(parent),
	Ui::SettingsDialog(),
	settings(settings),
	ftp_connect(0),
	ftp_login(0),
	ftp_close(0),
	ftp_upload(0),
	ftp_remove(0),
	ftp_remove_old_test(0),
	ftp(NULL) {
	Ui::SettingsDialog::setupUi(this);
	loadFromSettings();
    setupUi();
	connect(ftp_test_connection_button, SIGNAL(clicked()), SLOT(testFtpConnection()));
}

void SettingsDialog::setupUi() {

}

void SettingsDialog::testUpload() {
	if (ftp == NULL)
		return;

	ftp_upload = ftp->put(QByteArray("Imageprep test upload file"), "imageprep.test.upload.file", QFtp::Binary);
	ftp_remove = ftp->remove("imageprep.test.upload.file");
	ftp_close = ftp->close();
}

void SettingsDialog::loadFromSettings() {
	collectionsPathEdit->setText(settings->collectionsPath());
	ftp_host_name->setText(settings->ftpHost());
	ftp_user_name->setText(settings->ftpUser());
	ftp_password->setText(settings->ftpPassword());
}

void SettingsDialog::storeInSettings() {
	settings->setCollectionsPath(collectionsPathEdit->text());
	settings->setFtpHost(ftp_host_name->text());
	settings->setFtpUser(ftp_user_name->text());
	settings->setFtpPassword(ftp_password->text());
}

void SettingsDialog::testFtpConnection() {
	if (ftp != NULL)
		return;

	ftp = new QFtp(this);
	//qebug() << "Starting ftp connection test";
	connect(ftp, SIGNAL(commandFinished(int,bool)), SLOT(processFtpResult(int, bool)));
	connect(ftp, SIGNAL(commandStarted(int)), SLOT(showCurrentFtpAction(int)));
	ftp_test_connection_button->setIcon(QIcon(":/icons/spinner"));
	ftp_connect = ftp->connectToHost(ftp_host_name->text());
	ftp_login = ftp->login(ftp_user_name->text(), ftp_password->text());
	ftp_remove_old_test = ftp->remove("imageprep.test.upload.file");
}

void SettingsDialog::showCurrentFtpAction(int command) {
	if (command == ftp_connect)
		ftp_test_connection_button->setText(tr("Connecting..."));
	else if (command == ftp_login)
		ftp_test_connection_button->setText(tr("Logging in..."));
	else if (command == ftp_close)
		ftp_test_connection_button->setText(tr("Disconnecting..."));
	else if (command == ftp_upload)
		ftp_test_connection_button->setText(tr("Testing upload..."));
	else if (command == ftp_remove)
		ftp_test_connection_button->setText(tr("Removing test upload..."));
	else if (command == ftp_remove_old_test)
		ftp_test_connection_button->setText(tr("Removing old test upload..."));
}

void SettingsDialog::processFtpResult(int command, bool error) {
	//debug() << "Finished command" << command << "with status" << error;

	if (command == ftp_remove_old_test) {
		testUpload();
		return;
	}

	if (error) {
		ftp_test_connection_button->setIcon(QIcon(":/icons/ftp_error"));
		if (command == ftp_login)
			ftp_test_connection_button->setText(tr("Error logging-in. Check user/password."));
		else if (command == ftp_upload)
			ftp_test_connection_button->setText(tr("Error uploading file. Check permissions."));
		else if (command == ftp_remove)
			ftp_test_connection_button->setText(tr("Error removing uploaded file. Check permissions."));
		else if (command == ftp_connect)
			ftp_test_connection_button->setText(tr("Error connecting. Check host name."));
		ftp->deleteLater();
		ftp = NULL;
	}

	if (command == ftp_close) {
		if (not error) {
			ftp_test_connection_button->setIcon(QIcon(":/icons/ftp_ok"));
			ftp_test_connection_button->setText(tr("Connection OK."));
		}
		ftp->deleteLater();
		ftp = NULL;
	}
}

void SettingsDialog::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui.retranslateUi(this);
		break;
	default:
		break;
	}
}
