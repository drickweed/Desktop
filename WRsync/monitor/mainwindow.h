#pragma once

/** QT headers **/
#include <QCloseEvent>
#include <QTimer>

/** Application headers **/
#include "ui_mainwindow.h"
#include "monitor.h"
#include "trayicon.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT

	enum WRsyncState { OK, Running, Error };

	public:
		MainWindow(QWidget *parent = 0);

	private: // Properties
		TrayIcon *tray_icon;
		QTimer *icon_refresh_timer;
		bool last_checked_running;

	private: // Methods
		void createTrayIconMenu();
		void connectUi();

	private slots:
		void abort();
		void updateTrayIcon();
		void showAboutDialog();

	protected:
		void changeEvent(QEvent *e);

		void closeEvent(QCloseEvent *event);
};
