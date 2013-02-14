#include "mainwindow.h"

namespace FontMate {
	void MainWindow::setSearchResultCount(int count) {
		if (count == -1) {
			fontSearch->setMessageText("");
			return;
		}
		if (count == 0) {
			fontSearch->setMessageText("No fonts found");
			return;
		}
		fontSearch->setMessageText(QString("Found %1 fonts").arg(count));
	}
}
