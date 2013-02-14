#pragma once

#include "ui_monitorview.h"

class MonitorView : public QWidget, private Ui::MonitorView {
	Q_OBJECT
	public:
		MonitorView(QWidget *parent=0);

	protected:
		void changeEvent(QEvent *e);
};
