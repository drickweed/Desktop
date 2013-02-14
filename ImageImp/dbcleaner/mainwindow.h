#pragma once

/** QT headers **/
#include <QWidget>
#include <QPushButton>

// User Interface
#include "ui_cleaning.h"

namespace ImageImp {

	/**
		Database cleaning GUI.
	*/
	class MainWindow : public QWidget, private Ui::DBCleaner {

		Q_OBJECT

		public:
			MainWindow(QWidget *parent=0);

			QPushButton *closeButton;

		private:

			void connectUi();

			void setupUi();

	};

}
