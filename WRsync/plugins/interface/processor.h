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
#include "reporter.h"

/**
	Backup job processor interface

	Defines a destination for the backup and a means of copying the source files to it.
*/
class ProcessorInterface {

	public:

		enum RunState {None, Started, Done, FailedToStart, MissingDestination, InvalidSource, Error};

		/**
			Configuration UI widget.
		*/
		virtual QWidget *configUi() = 0;

		/**
			Plugin name for display porpuses.
		*/
		virtual QString name() const = 0;

		/**
			Unique plugin id. Used in config files.
		*/
		virtual QString id() const = 0;

		/**
			Save configuration from the user interface.
		*/
		virtual void saveConfig() = 0;

		/**
			Load configuration.
		*/
		virtual void loadConfig() = 0;

		/**
			Start processing the backup.

			@return True - running, False - no more sources to backup.
		*/
		virtual RunState run() = 0;

		/**
			User requested abort backup.
		*/
		virtual void abort() = 0;

		/**
			Check if backup is possible.
		*/
		virtual bool backupPossible() = 0;

		/**
			Set the backup srouces
		*/
		virtual void setSources(QStringList sources) = 0;

		virtual QList<Result*> results() const = 0;

		virtual Result* currentResult() const = 0;

		/**
			The actual backup command. Required for connecting signals and slots for reading stdout and stder.
		*/
		virtual QProcess *process() const = 0;

		virtual bool dryRun() const = 0;

		virtual void setDryRun(bool dry_run) = 0;

		virtual void initResult(EPath source) = 0;

		virtual void setResultState(Result::State state) = 0;

		virtual Result::State currentResultState() const = 0;

		/**
			Cleanup after job has ended (success is irrelevant)
		*/
		virtual bool jobCleanup() = 0;

		virtual void cleanupSource() = 0;

		virtual void setReporterPlugin(ReporterInterface *plugin) = 0;

};

Q_DECLARE_INTERFACE(ProcessorInterface, "com.jbox-comp.WRSync.ProcessorInterface/1.0")
