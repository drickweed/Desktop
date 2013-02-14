#pragma once

#include "ui_help.h"

#include "modelitems.h"
#include "model.h"

class HelpUi : public QDialog, private Ui::Help {
	
	Q_OBJECT
	
	public:
		HelpUi(QWidget* parent = 0);
};
