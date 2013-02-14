#include "configdialog.h"

/** QT headers **/
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QItemSelectionModel>
#include <QDesktopServices>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** Application headers **/
#include <wrsync/epath.h>
#include <wrsync/logfile.h>

using namespace JB;

namespace Local {

	ConfigDialog::ConfigDialog(QWidget *parent) :
		QWidget(parent){
		setupUi(this);

		connect(pathBrowseButton, SIGNAL(clicked()), SLOT(showSelectPathDialog()));
		connect(pathLineEdit, SIGNAL(textEdited(QString)), SLOT(updateCurrentDriveID()));
		connect(addIdButton, SIGNAL(clicked()), SLOT(addCurrentId()));
		connect(removeIdButton, SIGNAL(clicked()), SLOT(removeSelectedId()));
		connect(ejectScriptButton, SIGNAL(clicked()), SLOT(selectEjectScriptLocation()));

		deviceIdGroupBox->hide();
		deviceIdListView->setModel(&drive_ids);
	}

	void ConfigDialog::selectEjectScriptLocation() {
		QString default_script_file = Path::join(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation), "Eject backup devices.bat");
		QString file_name = QFileDialog::getSaveFileName(this, tr("Select location to save eject script"), default_script_file);

		LDEBUG_ << "Eject script file name " << QSTR(file_name);

		if (file_name.isEmpty())
			return;

		writeEjectScript(file_name);
	}

	void ConfigDialog::writeEjectScript(QString file_name) {
		LDEBUG_ << "Writing eject script to " << QSTR(file_name);
		QFile script_file(file_name);
		script_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
		QTextStream out(&script_file);
		QStringList commands;
		commands << qApp->applicationDirPath().left(2);
		commands << QString("cd %1").arg(QDir::toNativeSeparators(qApp->applicationDirPath()));
		foreach (QString id, drive_ids.stringList())
			commands << QString("cli\\usbtoolchest -e -d %1").arg(id);
		out << commands.join("\n");
		out.flush();
		script_file.close();
	}

	void ConfigDialog::addCurrentId() {
		if (currentIdLineEdit->text().isEmpty())
			return;

		QStringList ids = drive_ids.stringList();
		ids << currentIdLineEdit->text();
		drive_ids.setStringList(ids);
	}

	void ConfigDialog::removeSelectedId() {
		QItemSelectionModel *selection_model = deviceIdListView->selectionModel();
		QModelIndexList selection = selection_model->selectedRows(0);

		if (selection.empty())
			return;

		int index = selection.at(0).row();
		QStringList ids = drive_ids.stringList();
		ids.removeAt(index);
		drive_ids.setStringList(ids);
	}

	void ConfigDialog::loadConfig(Local::Settings *settings) {
		pathLineEdit->setText(settings->path());
		attachToPhysicalCheckBox->setChecked(settings->attachRemovable());
		drive_ids.setStringList(settings->ids());
		speedLimitSpinBox->setValue(settings->speedLimit());
		sshCommandLineEdit->setText(settings->sshCommand());
		deleteOldDestinationFilesCheckBox->setChecked((settings->deleteOldFiles()));
		rsyncCommandLineEdit->setText(settings->rsyncCommand());
		rsyncSwitchesLineEdit->setText(settings->rsyncSwitches());
		updateCurrentDriveID();
	}

	void ConfigDialog::saveConfig(Local::Settings *settings) {
		settings->setPath(pathLineEdit->text());
		settings->setAttachRemovable(attachToPhysicalCheckBox->isChecked());
		settings->setIds(drive_ids.stringList());
		settings->setSpeedLimit(speedLimitSpinBox->value());
		settings->setSshCommand(sshCommandLineEdit->text());
		settings->setRsyncCommand(rsyncCommandLineEdit->text());
		settings->setDeleteOldFiles(deleteOldDestinationFilesCheckBox->isChecked());
		settings->setRSyncSwitches(rsyncSwitchesLineEdit->text());
		settings->sync();
	}

	void ConfigDialog::showSelectPathDialog() {
		QString path = QFileDialog::getExistingDirectory(this, tr("Select destination backup path"), ".");

		if (path.isEmpty())
			return;

		pathLineEdit->setText(QDir::toNativeSeparators(path));
		updateCurrentDriveID();
	}

	void ConfigDialog::changeEvent(QEvent *e) {
		QWidget::changeEvent(e);
		switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
		}
	}

	void ConfigDialog::updateCurrentDriveID() {

		QString destination_path = pathLineEdit->text();

		if (destination_path.isEmpty()) {
			currentIdLineEdit->clear();
			m_last_destination_drive_letter = QChar::Null;
			return;
		}

		// Still on the same drive
		if (m_last_destination_drive_letter == destination_path[0]) {
			return;
		}

		m_last_destination_drive_letter = destination_path[0];

		EPath path(destination_path);

		qDebug() << "Updating current device ID";

		if (not path.path()[0].isLetter()) {
			qDebug() << "Invalid destination path";
			currentIdLineEdit->clear();
			return;
		}

		currentIdLineEdit->setText(path.driveID());
	}

	void ConfigDialog::updateDriveLetter() {

	}

}
