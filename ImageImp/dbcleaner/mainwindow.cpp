#include "mainwindow.h"

/** QT headers **/
#include <QVBoxLayout>

namespace ImageImp {

	MainWindow::MainWindow(QWidget* parent):
		QWidget(parent) {
		Ui_DBCleaner::setupUi(this);
		setupUi();
		connectUi();
	}

	void MainWindow::connectUi() {
	}

	void MainWindow::setupUi() {
		closeButton = new QPushButton(this);
		closeButton->setText("Done");

		dynamic_cast<QVBoxLayout*>(layout())->addWidget(closeButton);

		progressBar->hide();
	}

}
