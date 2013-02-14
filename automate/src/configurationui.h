#pragma once

#include "ui_configuration.h"

class ConfigurationUi : public QDialog, private Ui::Configuration {

	Q_OBJECT

	public:
		ConfigurationUi(QWidget* parent = 0);

	private slots:
		void save();
		void load();
		void selectDatabaseFolder();
		void clicked(QAbstractButton *button);
};

