#pragma once

/** QT headers **/
#include <QDialog>

/** JB headers **/
#include <jb/models/options.h>

// User Interface
#include "ui_config.h"

/** Application headers **/
#include "settings/config.h"

using namespace JB;

namespace ImageImp {

	/**
		ImageImp Config GUI.
	*/
	class MainWindow : public QDialog, private Ui::Config {
		Q_OBJECT

		public:
			MainWindow(QWidget *parent=0);

		private: /** Methods **/
			void connectUi();
			void setupModel();
			void setupUi();

		private: /** Properties **/

			ImageImp::Config config;
			OptionsModel *languages;

		private slots:
			void saveAndQuit();
			void quit();
			void save();
	};
}
