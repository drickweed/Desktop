#include "startup.h"

/** QT headers **/
#include <QStringList>
#include <QFile>

/** JB headers **/
#include <jb/qt4/pathutils.h>
#include <jb/qt4/app.h>

/** Application headers **/
#include <settings/system.h>

using namespace JB;

namespace ImageImp {

	void ensureDataFolders() {

		QStringList paths = QStringList() << System::pictureTemplatesPath() << System::albumTemplatesPath() << System::exportPath() << System::defaultPicturesPath() << System::scriptsPath();

		foreach(QString path, paths) {
			Path::ensurePath(path);
		}
	}

	void createDefaultTemplates() {
		// Ignoring the results of the copy operation. copy returns false if file exists and does not overwrite, which is what we want.
		//QFile(":/templates/album-template-001.psd").copy(Path::join(System::albumTemplatesPath(), "template-001.psd"));
		//QFile(":/templates/picture-template-001.psd").copy(Path::join(System::pictureTemplatesPath(), "template-001.psd"));
		//QFile(":/templates/picture-template-002.psd").copy(Path::join(System::pictureTemplatesPath(), "template-002.psd"));
	}

	void ensureFunctionalState() {
		ensureDataFolders();
		createDefaultTemplates();
		createPhotoshopScripts();
	}

	void createPhotoshopScripts() {
		//QFile(":/scripts/import.jsx").copy(Path::join(System::scriptsPath(), "import.jsx"));
		//QFile(":/scripts/import.vbs").copy(Path::join(System::scriptsPath(), "import.vbs"));
	}

}
