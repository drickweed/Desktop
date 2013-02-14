#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent){
	setupUi(this);
}

void AboutDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

void AboutDialog::closeEvent(QCloseEvent *event) {
	hide();
	event->ignore();
}
