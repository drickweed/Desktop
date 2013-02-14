#pragma once

/** QT headers **/
#include <QObject>
#include <QDateTime>

/** JB headers **/
#include <jb/database/object.h>

#include "global.h"

using namespace JB;

namespace WRsync {

	/**
		Represents a single instance of a backup job.
	*/
	class WRSYNC_API Backup : public RelationalObject {
		Q_OBJECT

		RELATIONAL_OBJECT(Backup);

		SIMPLE_PROPERTY(QDateTime, startedAT, setStartedAt, m_started_at);
		Q_PROPERTY(QDateTime started_at READ startedAT WRITE setStartedAt);

		public:
			explicit Backup(QObject *parent = 0);

			QString table() const { return "backup"; }

		signals:

		public slots:

	};
}
