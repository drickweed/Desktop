#pragma once

/** Application headers **/
#include <wrsync/epath.h>

/** QT headers **/
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDateTime>

#include "global.h"

const QString LOG_PATH = "log";

/**
	Backup result.

	Contains:
		- Backup process exit code
		- Internal state code
*/
class WRSYNC_API Result {

	public:

		/** Logging **/
		static void ensureBaseLogPath();

		enum State {None=				0x00,
					 OK=				0x01,
					 Error=				0x02,
					 UnknownError=		0x04,
					 MissingDestination=0x08,
					 MissingSource=		0x10,
					 FailedToStart=		0x20,
					 Crashed=			0x40,
					 Aborted=			0x80};

		static int AnyError;

		Result();
		Result(EPath source);

		void setSourcePath(EPath source);
		void setSourcePath(QString source);
		QString sourcePath() const { return m_source.realPath(); }

		void setExitCode(int exit_code);
		int exitCode() const { return m_exit_code; }

		void setState(State state) { m_state = state; }
		State state() const { return m_state; }

		QDateTime startedAt() const { return m_started_at; }
		void setStartedAt(QDateTime datetime) { m_started_at = datetime; }

		EPath destination() const { return m_destination; }
		void setDestination(QString destination) { m_destination = EPath(destination); }
		void setDestination(EPath destination) { m_destination = destination; }

		QByteArray toXml() const;

		/** Error log **/
		QString logPath() const;
		void appendErrorLog(QByteArray data);
		void appendLog(QByteArray data);

	private: // Properties
		int m_exit_code;
		State m_state;
		EPath m_source,
			  m_destination;
		QDateTime m_started_at;

		int m_output_handle, m_error_handle;
		QStringList read_failed_files;
		QStringList vanished_files;
		int m_error_count;


	private: // Methods
		QString xmlReport() const;
		void parseLogs();

		void init();
};
