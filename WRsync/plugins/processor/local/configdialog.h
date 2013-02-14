#pragma once

/** QT headers **/
#include <QSettings>

/** Application headers **/
#include "ui_configdialog.h"
#include "settings.h"
#include "driveidmodel.h"

namespace Local {

	class ConfigDialog : public QWidget, public Ui::ConfigDialog {
		Q_OBJECT
		public:
			ConfigDialog(QWidget *parent = 0);

			void saveConfig(Local::Settings *settings);
			void loadConfig(Local::Settings *settings);

		private: // Properties
			DriveIDModel drive_ids;
			QChar m_last_destination_drive_letter;

		private slots:
			void showSelectPathDialog();
			void updateCurrentDriveID();
			void updateDriveLetter();
			void addCurrentId();
			void removeSelectedId();
			void selectEjectScriptLocation();

		private: // Methods

			void writeEjectScript(QString file_name);

		protected:
			void changeEvent(QEvent *e);
	};

}
