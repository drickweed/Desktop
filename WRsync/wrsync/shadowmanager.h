#pragma once

/** QT headers **/
#include <QObject>
#include <QChar>
#include <QString>

/** Shadow volume manager **/
// #include <shadow.h>

namespace WRsync {

	class ShadowVolume : public QObject {
		Q_OBJECT
		public:
			ShadowVolume(QObject *parent=0);

			void start();
			void stop();
			QChar shadowDrive() const { return m_shadow_drive; }

			void setDrive(QChar real_drive) {
				m_drive = real_drive;
			}

		private: // Properties
			QChar m_drive; // Real drive letters for which shadow volumes were generated
			QChar m_shadow_drive; // <Real drive, Shadow drive>
			QString m_real_volume; // <Real drive, Real volume>
			QString m_shadow_volume; // <Real drive, Shadow volume>

		private: // Methods

//			JB::ShadowManager *shadow_manager;

			void mountGlobalVolume(QString volume, QChar drive);
			void umountGlobalVolume(QString volume);
			void umountGlobalVolume(QChar drive);

	};

}
