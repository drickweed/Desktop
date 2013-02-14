#pragma once

#include "ui_notifier.h"

class Notifier : public QWidget, private Ui::Notifier {
	Q_OBJECT
	public:
		Notifier(QWidget *parent = 0);

	public slots:
		void startBackup();

	protected:
		void changeEvent(QEvent *e);
};

