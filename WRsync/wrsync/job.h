#pragma once

/** QT headers **/
#include <QObject>

/** JB headers **/
#include <jb/database/object.h>

#include "global.h"

using namespace JB;

class WRSYNC_API Job : public RelationalObject {
	Q_OBJECT
	public:
		explicit Job(QObject *parent = 0);

	signals:

	public slots:

};

