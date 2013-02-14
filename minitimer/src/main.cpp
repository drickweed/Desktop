

#include <QApplication>
#include <QTimer>
#include <QSettings>

#include "timer.h"
#include "timerview.h"

using namespace MiniTimer;

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	app.setApplicationName("mini-timer");

	QSettings *settings = new QSettings("minitimer.ini", QSettings::IniFormat);

	Timer *timer = new Timer;
	timer->setTime(200);
	timer->start();

	TimerView *timer_view = new TimerView;
	timer_view->setTimer(timer);
	timer_view->loadState(settings);

	timer_view->show();

	int result = app.exec();

	timer_view->saveState(settings);
	settings->sync();

	return result;
}

