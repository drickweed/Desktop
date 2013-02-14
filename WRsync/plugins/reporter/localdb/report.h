#pragma once

/** JB headers **/
#include <jb/database/object.h>

/** QT headers **/
#include <QString>
#include <QDateTime>

/** Application headers **/
#include <wrsync/result.h>

using namespace JB;

class Report : public RelationalObject {
	Q_OBJECT
	RELATIONAL_OBJECT(Report)

	SIMPLE_PROPERTY(QString, source, setSource, m_source);
	Q_PROPERTY(QString source READ source WRITE setSource)

	SIMPLE_PROPERTY(int, exitCode, setExitCode, m_exit_code);
	Q_PROPERTY(int exitcode READ exitCode WRITE setExitCode)

	SIMPLE_PROPERTY(int, state, setState, m_state);
	Q_PROPERTY(int state READ state WRITE setState)

	SIMPLE_PROPERTY(QDateTime, startedAt, setStartedAt, m_started_at);
	Q_PROPERTY(QDateTime started_at READ startedAt WRITE setStartedAt)

	SIMPLE_PROPERTY(QString, destinationId, setDestinationId, m_destination_id);
	Q_PROPERTY(QString destination_id READ destinationId WRITE setDestinationId)

	SIMPLE_PROPERTY(QString, destinationPath, setDestinationPath, m_destination_path);
	Q_PROPERTY(QString destination_path READ destinationPath WRITE setDestinationPath)

	public:
		QString table() const { return "reports"; }
		QString fieldName() const { return "report"; }

		Result toResult() const;

		void fromResult(Result result);


};
