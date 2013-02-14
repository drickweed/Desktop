#include "configurationui.h"

#include <QFileDialog>

#include "config.h"
#include "const.h"

ConfigurationUi::ConfigurationUi(QWidget* parent) : QDialog(parent) {
	setupUi(this);
	setModal(true);
	connect(database_browse_button, SIGNAL( clicked() ), this, SLOT(selectDatabaseFolder()));
	connect(this, SIGNAL(accepted()), this, SLOT(save()));
	connect(confirm_buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));
	load();
}

void ConfigurationUi::load() {
	database_lineEdit->setText(CONFIG.database().c_str());
	overwrite_checkBox->setCheckState(CONFIG.overwrite() ? Qt::Checked : Qt::Unchecked);
	instructions_checkBox->setCheckState(CONFIG.showHelp() ? Qt::Checked : Qt::Unchecked);
	haltOn_comboBox->setCurrentIndex(static_cast<int>(CONFIG.stopProcessingOn()));
	silentFirst_comboBox->setCurrentIndex(CONFIG.silent());
}

void ConfigurationUi::save() {
	CONFIG.setDatabase(database_lineEdit->text().toStdString());
	CONFIG.setOverwrite(overwrite_checkBox->isChecked());
	CONFIG.setShowHelp(instructions_checkBox->isChecked());
	CONFIG.setStopProcessingOn(static_cast<ProcessingError::ProcessingErrorType>(haltOn_comboBox->currentIndex()));
	CONFIG.setSilent(static_cast<SilentOrder::SilentOrderTypes>(silentFirst_comboBox->currentIndex()));
}

void ConfigurationUi::clicked(QAbstractButton *button) {
	QDialogButtonBox::StandardButton role = confirm_buttonBox->standardButton(button);
	switch (role) {
		case QDialogButtonBox::RestoreDefaults:
			CONFIG.restoreDefaults();
			load();
			break;
		default:
			break;
	}
}

void ConfigurationUi::selectDatabaseFolder() {
	QString dir = QFileDialog::getExistingDirectory(this, "Open Database Directory"/*,"/home",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks*/);
	if(dir != "" )
		database_lineEdit->setText(dir);
}
