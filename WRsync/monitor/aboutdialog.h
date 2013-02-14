#pragma once

/** QT headers **/
#include <QCloseEvent>

#include "ui_aboutdialog.h"

class AboutDialog : public QDialog, private Ui::AboutDialog {
	Q_OBJECT
public:
	AboutDialog(QWidget *parent = 0);

protected:
	void changeEvent(QEvent *e);

	void closeEvent(QCloseEvent *event);
};
