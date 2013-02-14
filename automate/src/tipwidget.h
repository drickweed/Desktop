#pragma once

#include <QWidget>
#include <QString>

#include "ui_tip.h"

class TipWidget : public QWidget, private Ui::Tip {

	public:
		TipWidget(QString text, QWidget *parent=0);

		void setText(QString text) { tip_text->setText(text); }

};
