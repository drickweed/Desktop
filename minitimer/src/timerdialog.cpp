#include "timerdialog.h"

namespace MiniTimer {

	TimerDialog::TimerDialog(QWidget* parent, Qt::WFlags fl):
		QDialog(parent, fl),
		Ui::Dialog() {
		setupUi(this);
		setWindowFlags(Qt::Dialog);
	}

	TimerDialog::~TimerDialog() {
	}

	void TimerDialog::reject() {
		QDialog::reject();
	}

	void TimerDialog::accept() {
		QDialog::accept();
	}

}
