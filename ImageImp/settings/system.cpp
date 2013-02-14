#include "system.h"

/** JB headers **/
#include <jb/qt4/app.h>
#include <jb/qt4/pathutils.h>

/** QT headers **/
#include <QFileInfo>

/** Application headers **/
#include "config.h"

using namespace JB;

namespace ImageImp {

	QString System::m_data_path;

	QString System::pictureTemplatesPath() {
		return Path::join(templatesPath(), "picture");
	}

	QString System::albumTemplatesPath() {
		return Path::join(templatesPath(), "album");
	}

	void System::init() {
		m_data_path = QFileInfo(QString(App::path().c_str())).absoluteFilePath();
	}

	QString System::templatesPath() {
		return Path::join(dataPath(), "data/templates");
	}

	QString System::exportPath() {
		return Path::join(dataPath(), "export");
	}

	QString System::dataPath() {
		return m_data_path;
	}

	QString System::databasePath() {
		return Path::join(dataPath(), "imageimp.sqlite3");
	}

	QString System::defaultPicturesPath() {
		return Path::join(dataPath(), "data/pictures");
	}

	QString System::scriptsPath() {
		return Path::join(dataPath(), "scripts");
	}

}

QString ImageImp::System::translationFile() {
	Config config;
	return Path::join(dataPath(), QString("imageimp_%1").arg(config.language()));
}

bool ImageImp::System::rightToLeftLanguage() {
	Config config;
	return (config.language().compare("he") == 0);
}
