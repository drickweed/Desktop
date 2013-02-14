#pragma once

/** QT heders **/
#include <QStringList>

class Options {
	public:
		Options();
		Options(QStringList arguments);

	private:
		void loadDefaults();
		void initFromArguments(QStringList arguments);

	public:
		bool dry_run,
			 check_state;
		QString job_name;
};

