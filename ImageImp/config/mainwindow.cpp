#include "mainwindow.h"

/** QT headers **/
#include <QDebug>

namespace ImageImp {

	MainWindow::MainWindow(QWidget* parent):
		QDialog(parent){
		Ui_Config::setupUi(this);

		setupModel();
		setupUi();
		connectUi();

	}

	void MainWindow::connectUi() {
		connect(buttonBox, SIGNAL(accepted()), SLOT(saveAndQuit()));
		connect(buttonBox, SIGNAL(rejected()), SLOT(quit()));
	}

	void MainWindow::setupUi() {
		/** Language **/
		languageSelection->setModel(languages);
		int language_index = languages->indexOfValue(config.language());
		if (language_index < 0) language_index = 0;
		languageSelection->setCurrentIndex(language_index);

		/** Logging **/
		errorLogging->setChecked(config.logging());

	}
}

void ImageImp::MainWindow::setupModel() {
	languages = new OptionsModel(this);
	languages->setDisplayItems(QStringList() << "English" << "Hebrew" );
	languages->setValueItems(QStringList() << "en" << "he");
}

void ImageImp::MainWindow::saveAndQuit() {
	save();
	accept();
}

void ImageImp::MainWindow::quit() {
	reject();
}

void ImageImp::MainWindow::save() {
	config.setLanguage(languages->valueAt(languageSelection->currentIndex()));
	config.setLogging(errorLogging->isChecked());
	config.sync();
}
