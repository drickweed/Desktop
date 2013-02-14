#include "target.h"

/** QT headers **/
#include <QDir>
#include <QFile>

using namespace JB;

Target::Target() {
}

Target::Target(QString path):
	m_path(path) {
	Path::ensurePath(path);
}

QString Target::name() const {
	if (path() != QDir::rootPath())
		return QFileInfo(path()).fileName();
	else
		return "Root folder";
}
