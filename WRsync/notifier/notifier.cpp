#include "notifier.h"

Notifier::Notifier(QWidget *parent) :
	QWidget(parent){
	setupUi(this);
}

void Notifier::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

void Notifier::startBackup() {

}
