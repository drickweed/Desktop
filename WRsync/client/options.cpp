#include "options.h"

/** Application headers **/
#include <wrsync/logfile.h>

Options::Options() {
	loadDefaults();
}

Options::Options(QStringList arguments) {
	loadDefaults();
	initFromArguments(arguments);
}

void Options::loadDefaults() {
	dry_run = false;
	check_state = false;
	job_name = QString();
}

void Options::initFromArguments(QStringList arguments) {
	if (arguments.contains("-n")) {
		dry_run = true;
		LAPP_ << "Set to run in dry mode";
	}

	if (arguments.contains("-c")) {
		check_state = true;
		LAPP_ << "Set to only check if backup can be run ok";
	}
}
