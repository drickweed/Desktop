#include "sourcemanager.h"

#include "epath.h"

SourceManager::SourceManager(QObject *parent) :
	QObject(parent)
{
}

void SourceManager::setSources(QStringList sources) {
	m_sources = sources;
	collectLocalDrives();
}

void SourceManager::collectLocalDrives() {
	foreach (QString source, m_sources) {
		EPath path(source);
		if (path.isValid() and path.isLocalPath())
			m_source_drives.insert(path.realDriveLetter());
	}
}

QStringList SourceManager::paths() const {
	QStringList real_paths;
	foreach(QString path, m_sources) {
		if (EPath(path).isLocalPath())
			real_paths.append(path);
	}
	return real_paths;
}
