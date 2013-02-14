#pragma once
#include "ui_ibackupconfigdialog.h"
#include "settings.h"

/** QT headers **/
#include <QHttp>

class IBackupConfigDialog : public QWidget, private Ui::IBackupConfigDialog {
	Q_OBJECT
public:
	IBackupConfigDialog(QWidget *parent = 0);

	void loadSettings(Settings *settings);
	void saveSettings(Settings *settings);

private slots:
	void verifyConnection();
	void processResult(bool error);

protected:
	void changeEvent(QEvent *e);

private:
	QHttp *validationRequest;
};

