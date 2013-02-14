#pragma once

#include "ui_localconfigdialog.h"

class LocalConfigDialog : public QWidget, private Ui::LocalConfigDialog {
	Q_OBJECT
public:
	LocalConfigDialog(QWidget *parent = 0);

protected:
	void changeEvent(QEvent *e);
};
