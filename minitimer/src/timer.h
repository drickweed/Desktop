#pragma once

/** QT headers **/
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QString>
//#include <phonon/AudioOutput>
//#include <phonon/MediaObject>

namespace MiniTimer {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Timer : public QObject {
		Q_OBJECT
		public:
			Timer(QObject *parent=0);

			virtual ~Timer();

			int timePassed() const { return m_start_time.secsTo(QTime::currentTime()); }

			int timeLeft() const { return time() - timePassed(); }

			int time() const { return m_time; }
			void setTime(int time) { m_time = time; }
			void setTime(int h, int m, int s) { m_time = h * 3600 + m * 60 + s; }

			void setTickRate(int seconds) { m_tick_rate = seconds; }
			int tickRate() const { return m_tick_rate; }

			void start();
			void pause();
			void stop();
			void reset();

			bool paused() const { return m_paused; }

		signals:
			void tick(int time_passed);
			void finished();

		private slots:
			void sendTick();

			void playTimerEndNotification();

		private:
			int m_time;
			int m_tick_rate; ///< Seconds before tick emitted.

			QString m_action_text;

			QTimer *tick_timer;
			QTime m_start_time;
			QTime m_pause_start_time;

			bool m_paused;

//			Phonon::MediaObject *mediaObject;
//			Phonon::AudioOutput *audioOutput;
//			Phonon::Path audioPath;
	};

}
