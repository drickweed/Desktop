#pragma once

/** Boost headers **/
#include <boost/filesystem.hpp>

/** QT headers **/
#include <QProcess>
#include <QObject>
#include <QMap>

/** STL headers **/
#include <string>
#include <exception>

#include "version.h"

/**
	Utility class that installs applications.
*/
class Installer : public QObject {

	Q_OBJECT

	static QMap<QProcess::ProcessError, QString> ErrorMessage;

	private:

		/**
			Set to true when installer is perfoming installation.
		*/
		bool busy;

		/**
			Exit code of last operation.
		*/
		int lastcode;

		/**
			Name of last installed Application.
		*/
		std::string lastApp;

		/**
			Internal installation function.

			@param app Version to install.
			@param dryrun Won't execute the installer if true.
		*/
		void run(Version app, bool dryrun);

		QProcess *installer;

	public:

		Installer(QObject *parent=0);

		virtual ~Installer();

		/**
			Installs the given version.

			@param app Version to install.
			@param dryrun Passed to runInstaller.
		*/
		void install(Version app, bool dryrun=false);

		/**
			Returns exit code of last installation.

			@return Exit code.
		*/
		int lastCode() { return lastcode; }

		/**
			Exception. Installer is busy.
		*/
		class Busy : public std::exception {};

	signals:
		void done();

	private:
		void commonCleanup();

	private slots:
		void cleanupFinished(int result, QProcess::ExitStatus status);
		void cleanupError(QProcess::ProcessError error);
};
