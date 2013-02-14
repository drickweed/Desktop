#include "diskspace.h"

/** QT headers **/
#include <QDir>
#include <QtPlugin>
#include <QByteArray>

/** JB headers **/
#include <jb/qt4/strings.h>

/** Application headers **/
#include <wrsync/exceptions.h>
#include "configdialog.h"

#define PATH_MACRO(name, value) e_path = e_path.replace(name, value);

using namespace JB;

namespace DiskSpace {

	Monitor::Monitor():
		QObject(),
		m_config_ui(NULL) {

		m_settings = new DiskSpace::Settings(this);

		loadConfig();
	}

	Monitor::~Monitor() {
		LPLUGIN_ << QSTR(name()) << " Saving plugin setings";
		saveConfig();
	}

	void Monitor::saveConfig() {

	}

	void Monitor::loadConfig() {

	}

	QWidget* Monitor::configUi() {
		if (m_config_ui == NULL) {
			m_config_ui = new ConfigDialog();
			loadConfig();
		}

		return m_config_ui;
	}

}

Q_EXPORT_PLUGIN2(monitor_diskspace, DiskSpace::Monitor)
