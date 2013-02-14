#pragma once

/** JB headers **/
#include <jb/database/object.h>

using namespace JB;

class SystemLogEntry : public RelationalObject {
	Q_OBJECT
	RELATIONAL_OBJECT(SystemLogEntry);

	public:

		QString table() const { return "system_log"; }

};
