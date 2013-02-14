#pragma once

/** QT headers **/
#include <QString>

#include "ui_collectionnamedialog.h"

class CollectionNameDialog : public QDialog, private Ui::CollectionNameDialog {
	Q_OBJECT
	public:
		CollectionNameDialog(QWidget *parent=0);

		static QString getName();

	protected:
		virtual void changeEvent(QEvent *e);
};

