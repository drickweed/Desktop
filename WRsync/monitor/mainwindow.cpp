#include "mainwindow.h"

/** QT headers **/
#include <QMenu>

/** JB headers **/
#include <jb/qt4/fileutils.h>

/** Application headers **/
#include "aboutdialog.h"

using namespace JB;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	last_checked_running(false) {
	setupUi(this);

	icon_refresh_timer = new QTimer(this);
	icon_refresh_timer->setInterval(5000);

	tray_icon = new TrayIcon(this);
	QIcon icon(":/icons/tray.png");
	tray_icon->setIcon(icon);
	createTrayIconMenu();

	connectUi();

	tray_icon->show();

	icon_refresh_timer->start();
}

void MainWindow::connectUi() {
	connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(actionSettings, SIGNAL(triggered()), SLOT(show()));
	connect(actionAbort, SIGNAL(triggered()), SLOT(abort()));
	connect(icon_refresh_timer, SIGNAL(timeout()), SLOT(updateTrayIcon()));
	connect(actionAbout, SIGNAL(triggered()), SLOT(showAboutDialog()));
}

void MainWindow::showAboutDialog() {
	AboutDialog dialog;
	dialog.exec();
}

void MainWindow::updateTrayIcon() {
	bool running = Monitor().backupRunning();

	if (running == last_checked_running)
		return;

	last_checked_running = running;
	if (not running) {
		QIcon icon(":/icons/tray.png");
		tray_icon->setIcon(icon);
	} else {
		QIcon icon(":/icons/running.png");
		tray_icon->setIcon(icon);
	}
}

void MainWindow::closeEvent(QCloseEvent *event) {
	hide();
	event->ignore();
}

void MainWindow::createTrayIconMenu() {
	QMenu *menu = new QMenu(this);
	// menu->addAction(actionSettings);
	menu->addAction(actionAbort);
	menu->addAction(actionAbout);
	menu->addSeparator();
	menu->addAction(actionQuit);

	tray_icon->setContextMenu(menu);
}

void MainWindow::abort() {
	Monitor().stopBackup();
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}
