#include "mainwindow.h"

/** QT headers **/
#include <QMessageBox>

namespace FontMate {

	void MainWindow::updateInstallButton() {

		switch (fonts()->collectionSource()) {
			case Fonts::None:
					break;
			case Fonts::Installed:
					uninstallSelectionButton->setHidden(fonts()->selectionEmpty());
					installSelectionButton->hide();
					break;
			case Fonts::Folder:
					uninstallSelectionButton->setVisible(fonts()->installedFontsSelected());
					installSelectionButton->setHidden(fonts()->selectionEmpty());
					break;
		}
	}

	void MainWindow::installSelection() {
		QMessageBox::StandardButton result = QMessageBox::question(this, tr("Install selected fonts ?"), tr("Install selected fonts ?"), QMessageBox::Yes | QMessageBox::No);
		if (result == QMessageBox::Yes)
			fonts()->installSelection();
	}

	void MainWindow::uninstallSelection() {
		QMessageBox::StandardButton result = QMessageBox::question(this, tr("Uninstall selected fonts ?"), tr("Uninstall selected fonts ?"), QMessageBox::Yes | QMessageBox::No);
		if (result == QMessageBox::Yes)
			fonts()->uninstallSelection();
	}

}
