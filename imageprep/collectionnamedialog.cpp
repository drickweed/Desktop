#include "collectionnamedialog.h"

CollectionNameDialog::CollectionNameDialog(QWidget *parent) :
	QDialog(parent){
	setupUi(this);
}

void CollectionNameDialog::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

QString CollectionNameDialog::getName() {
	CollectionNameDialog dialog;
	int result = dialog.exec();
	switch (result) {
		case QDialog::Accepted:
			return dialog.collectionNameEdit->text();
			break;
		case QDialog::Rejected:
			return "";
			break;
	};
	return "";
}
