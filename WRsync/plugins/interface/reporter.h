#pragma once

/** QT headers **/
#include <QWidget>
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QList>

/** Application headers **/
#include <wrsync/result.h>
#include <wrsync/epath.h>

class ReporterInterface {

	public:

		/**
			Configuration user interface.
		*/
		virtual QWidget *configUi() = 0;

		/**
			Plugin name.

			Used in display and human readable logs.
		*/
		virtual QString name() const = 0;

		/**
			Plugin identifier.

			Used in config files. Usually in the form of a UUID.
		*/
		virtual QString id() const = 0;

		/**
			Save plugin config.
		*/
		virtual void saveConfig() = 0;

		/**
			Load plugin config.
		*/
		virtual void loadConfig() = 0;

		/**
			Add report record to database.
		*/
		virtual bool report(Result result) = 0;

		virtual QMap<QDateTime, int> resultsForMonth(int month, int year=QDate::currentDate().year()) const = 0;

		virtual QList<Result> resultsForDay(QDate date) const = 0;

		virtual EPath findLastValidForSource(QString source, QString destination_id) const = 0;

};

Q_DECLARE_INTERFACE(ReporterInterface, "com.jbox-comp.WRSync.ReporterInterface/1.0")
