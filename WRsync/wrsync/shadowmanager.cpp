#include "shadowmanager.h"

#include <QDebug>

namespace WRsync {

	ShadowVolume::ShadowVolume(QObject *parent) :
		QObject(parent) {
	}

	void ShadowVolume::start() {
//		shadow_manager = new JB::ShadowManager();
//		shadow_manager->setRealDrive(m_drive.toAscii());
//		shadow_manager->mount();
	}

	void ShadowVolume::stop() {
//		shadow_manager->umount();
//		delete shadow_manager;
	}

}
