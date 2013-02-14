#include "mainwindow.h"

/** QT headers **/
#include <QFileDialog>

namespace FontMate {

	void MainWindow::loadFontPaths() {
		foreach(QString path, settings->fontPaths()) {
			if (not QDir(path).exists())
				continue;

			fontCollection->insertItem(1, path);
		}
	}

	void MainWindow::saveFontPaths() {
		settings->setFontPaths(folders()->paths());
		settings->sync();
	}

	void MainWindow::addFolder() {
		QString path = QFileDialog::getExistingDirectory(this, tr("Select folder with fonts to load"));

		if (path.isEmpty())
			return;

		folders()->addFolder(path);
	}

}
