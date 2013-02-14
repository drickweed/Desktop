#pragma once

/** QT headers **/
#include <QWidget>
#include <QString>
#include <QSettings>

/** Application headers **/
#include <interface/destination.h>
#include "localconfigdialog.h"

class LocalDestination: public QObject, public DestinationInterface {
	Q_OBJECT
	Q_INTERFACES(DestinationInterface)

	public:
		LocalDestination();
		~LocalDestination();

		QString name() const { return "Local disk"; }
		QString id() const { return "e733ecd0-1d7e-11df-b05c-0016d3298e6d"; }

		QWidget *configUi();

		void saveConfig();
		void loadConfig();

		QString fullPath() const { return m_path; }

	private:

		LocalConfigDialog *m_config_ui;

		QString m_path;
		QSettings *settings;

};


