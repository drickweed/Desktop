#pragma once

/** QT headers **/
#include <QWidget>
#include <QString>

class DestinationInterface {

	public:
		virtual QWidget *configUi() = 0;

		virtual QString name() const = 0;
		virtual QString id() const = 0;

		virtual void saveConfig() = 0;
		virtual void loadConfig() = 0;

		virtual bool fullPath() = 0;

};

Q_DECLARE_INTERFACE(DestinationInterface, "com.jbox-comp.WRSync.DestinationInterface/1.0")
