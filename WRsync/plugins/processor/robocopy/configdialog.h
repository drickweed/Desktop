#pragma once

/** QT headers **/
#include <QSettings>

/** Application headers **/
#include "ui_configdialog.h"
#include "settings.h"

class RSyncConfigDialog : public QWidget, public Ui::RSyncConfigDialog {
	Q_OBJECT
	public:
		RSyncConfigDialog(QWidget *parent = 0);

		void saveConfig(RSync::Settings *settings);
		void loadConfig(RSync::Settings *settings);

	private slots:
		void showSelectKeyDialog();

	protected:
		void changeEvent(QEvent *e);
};
