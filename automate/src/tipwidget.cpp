#include "tipwidget.h"

#include <QSpacerItem>
#include <QHBoxLayout>

TipWidget::TipWidget(QString text, QWidget *parent):
	QWidget(parent) {
	tip_text->setText(text);
}
