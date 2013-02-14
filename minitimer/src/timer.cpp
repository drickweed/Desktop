#include "timer.h"

/** QT headers **/
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QDebug>

namespace MiniTimer {

	Timer::Timer(QObject *parent):
		QObject(parent),
		m_tick_rate(5),
		m_paused(false) {

		// QTimer MUST have parent. otherwise irregular qt crash.
		tick_timer = new QTimer(this);
		connect(tick_timer, SIGNAL(timeout()), SLOT(sendTick()));
		connect(this, SIGNAL(finished()), SLOT(playTimerEndNotification()));

//		mediaObject = new Phonon::MediaObject(this);
//		mediaObject->setCurrentSource(Phonon::MediaSource(":/sounds/timer-end.wav"));
//		audioOutput = new Phonon::AudioOutput(Phonon::NotificationCategory, this);
//		audioPath = Phonon::createPath(mediaObject, audioOutput);
	}

	Timer::~Timer() {
//		mediaObject->deleteLater();
//		audioOutput->deleteLater();
	}

	void Timer::start() {
		reset();
		tick_timer->setInterval(tickRate()*100);
		tick_timer->start();
		sendTick();
	}

	void Timer::pause() {
		m_pause_start_time = QTime::currentTime();
		m_paused = not m_paused;
	}

	void Timer::stop() {
		tick_timer->stop();
	}

	void Timer::reset() {
		m_start_time = QTime::currentTime();
	}

	void Timer::sendTick() {
		QTime mark = QTime::currentTime();

		if (not paused()) {
			if (timeLeft() <= 0) {
				stop();
				emit finished();
			}
				emit tick(m_start_time.secsTo(mark));
		} else {
			int pause_interval = m_pause_start_time.msecsTo(mark);
//			qDebug() << "Paused for" << pause_interval << "secs";
			m_start_time = m_start_time.addMSecs(pause_interval);
			m_pause_start_time = mark;
		}
	}

	void Timer::playTimerEndNotification() {
//		mediaObject->play();
	}
}

