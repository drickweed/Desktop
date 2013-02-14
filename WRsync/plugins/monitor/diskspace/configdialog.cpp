#include "configdialog.h"

/** QT headers **/
#include <QFileDialog>
#include <QDebug>
#include <QItemSelectionModel>

/** Application headers **/
#include <wrsync/epath.h>

namespace DiskSpace {

	ConfigDialog::ConfigDialog(QWidget *parent) :
		QWidget(parent){
		setupUi(this);

	}

	void ConfigDialog::changeEvent(QEvent *e) {
		QWidget::changeEvent(e);
		switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
		}
	}

}
