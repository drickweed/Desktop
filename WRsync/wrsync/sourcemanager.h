#pragma once

/** QT headers **/
#include <QObject>
#include <QList>
#include <QSet>
#include <QChar>
#include <QStringList>

class SourceManager : public QObject {
	Q_OBJECT
	public:
		SourceManager(QObject *parent=0);

		void setSources(QStringList sources);
		QList<QChar> localDrives() const { return m_source_drives.toList(); }
		QStringList paths() const;

	private:
		QSet<QChar> m_source_drives;
		QStringList m_sources;

	private: // Methdos
		void collectLocalDrives();
};

