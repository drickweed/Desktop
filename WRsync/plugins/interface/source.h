#pragma once

/** QT headers **/
#include <QWidget>
#include <QString>

/** Application headers **/
#include <wrsync/rsyncresult.h>

class ReporterInterface {

	public:
		virtual QWidget *configUi() = 0;

		virtual QString name() const = 0;
		virtual QString id() const = 0;

		virtual void saveConfig() = 0;
		virtual void loadConfig() = 0;

		virtual  bool report(RSyncResult result) = 0;

};

Q_DECLARE_INTERFACE(ReporterInterface, "com.jbox-comp.WRSync.ReporterInterface/1.0")
