#include <QtCore/QCoreApplication>

/** Application headers **/
#include <wrsync/schedule.h>
#include <wrsync/logfile.h>

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);

	init_logs();

	Schedule schedule;
	CronString cron;
	//cron.tokenize("0 0 * * * command line");
	//cron.tokenize("0 0 */2 1,5 */2 command line");
	cron.tokenize("0 0 * * sun command line");

	foreach(CronToken token, cron.tokens)
		LDEBUG_ << QSTR(token.toString());

	schedule.build(cron.tokens);

	LDEBUG_ << QSTR(schedule.toString());
	//return a.exec();
	return 0;
}
