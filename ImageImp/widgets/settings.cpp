#include "mainwindow.h"

#include "settings/export.h"

namespace ImageImp {

	void MainWindow::connectSettingsUi() {
		connect(albumHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(setHeight(int)));
		connect(albumWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(setWidth(int)));
	}

	void MainWindow::setWidth(int i) {
		ExportSettings::setWidth(i);
	}

	void MainWindow::setHeight(int i) {
		ExportSettings::setHeight(i);
	}
}
