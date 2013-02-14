#include "configdialog.h"

/** QT headers **/
#include <QFileDialog>

RSyncConfigDialog::RSyncConfigDialog(QWidget *parent) :
	QWidget(parent){
	setupUi(this);

	connect(keyBrowseButton, SIGNAL(clicked()), SLOT(showSelectKeyDialog()));
}

void RSyncConfigDialog::loadConfig(RSync::Settings *settings) {
	hostnameLineEdit->setText(settings->hostname());
	usernameLineEdit->setText(settings->username());
	passwordLineEdit->setText(settings->password());
	pathLineEdit->setText(settings->path());
	keyPathLineEdit->setText(settings->keyPath());
}

void RSyncConfigDialog::saveConfig(RSync::Settings *settings) {
	settings->setHostname(hostnameLineEdit->text());
	settings->setUsername(usernameLineEdit->text());
	settings->setPassword(passwordLineEdit->text());
	settings->setPath(pathLineEdit->text());
	settings->setKeyPath(keyPathLineEdit->text());
	settings->sync();
}

void RSyncConfigDialog::showSelectKeyDialog() {
	QString key_file = QFileDialog::getOpenFileName(this, "Select SSH private key", ".");

	if (key_file.isEmpty())
		return;

	keyPathLineEdit->setText(key_file);
}

void RSyncConfigDialog::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}
