#include "menubar.h"

/** QT headers **/
#include <QMenuBar>
#include <QAction>

/** Application headers **/
#include "mainwindow.h"

using namespace FontMate;

namespace osx {

	MenuBar::MenuBar(QObject *parent):
		QObject(parent) {
	}


	void MenuBar::generate(QWidget *parent) {
		MainWindow *window = qobject_cast<MainWindow*>(parent);

		if (not window)
			return;

		// Hide "settings" and "about" toolbar buttons
		window->settingsButton->hide();
		window->aboutButton->hide();

		// Add menu items and connect actions
		QMenuBar *menuBar = window->menuBar();
		QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
		helpMenu->addAction(window->actionAbout);

		QMenu *fileMenu = menuBar->addMenu(tr("&File"));
		fileMenu->addAction(window->actionSettings);

	}

}
