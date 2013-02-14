#pragma once

#include "settings.h"
#include "ui_settingsdialog.h"

#include <QFtp>

class SettingsDialog : public QDialog, private Ui::SettingsDialog {
	Q_OBJECT
	public:
		SettingsDialog(Settings *settings, QWidget *parent=0);
		void storeInSettings();

	protected:
		virtual void changeEvent(QEvent *e);
		void loadFromSettings();
        void setupUi();

	private:
		Settings *settings;
		Ui::SettingsDialog m_ui;

		// FTP states
		int ftp_connect,
			ftp_login,
			ftp_close,
			ftp_upload,
			ftp_remove,
			ftp_remove_old_test;

		QFtp *ftp;

		bool ranUploadText() const { return ftp_upload != 0; }

	private slots:
		void testFtpConnection();
		void testUpload();
		void processFtpResult(int command, bool error);
		void showCurrentFtpAction(int command);
};
