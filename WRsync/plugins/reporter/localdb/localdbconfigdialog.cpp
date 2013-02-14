#include "localdbconfigdialog.h"

/** QT headers **/
#include <QFileDialog>

namespace LocalDb {

	ConfigDialog::ConfigDialog(QWidget *parent) :
		QWidget(parent){
		setupUi(this);
		connect(resetButton, SIGNAL(clicked()), SIGNAL(resetDatabase()));
		connect(browseButton, SIGNAL(clicked()), SLOT(showSelectDatabaseDialog()));
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

	void ConfigDialog::loadSettings(Settings *settings) {
		pathLineEdit->setText(settings->path());
	}

	void ConfigDialog::saveSettings(Settings *settings) {
		settings->setPath(pathLineEdit->text());
		settings->sync();
	}

	void ConfigDialog::showSelectDatabaseDialog() {
		QString database_file_name = QFileDialog::getSaveFileName(this, "Select backup log database", ".");

		if (database_file_name.isEmpty())
			return;

		pathLineEdit->setText(database_file_name);
	}
}
