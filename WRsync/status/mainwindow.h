#pragma once

#include "ui_mainwindow.h"
#include <jb/widgets/filelisteditor.h>

using namespace JB;

class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
	public:
		MainWindow(QWidget *parent = 0);

	protected:
		void changeEvent(QEvent *e);

	private:
		FileListEditor *viewer;

		void setupUi();
};

