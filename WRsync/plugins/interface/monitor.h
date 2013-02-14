#pragma once

/** QT headers **/
#include <QWidget>
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QList>
#include <QProcess>

/** Application headers **/
#include <wrsync/result.h>
#include <wrsync/epath.h>

class MonitorInterface {

	public:

		enum Levels {OK, Warning, Fatal};

		virtual QWidget *configUi() = 0;

		virtual QString name() const = 0;
		virtual QString id() const = 0;

		virtual void saveConfig() = 0;
		virtual void loadConfig() = 0;

		/**
			Monitor message;
		*/
		virtual QString message() const = 0;

		/**
			Message level
		*/
		virtual int level() const = 0;

};

Q_DECLARE_INTERFACE(MonitorInterface, "com.jbox-comp.WRSync.MonitorInterface/1.0")
