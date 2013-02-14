#pragma once

/** QT headers **/
#include <QObject>
#include <QProcess>
#include <QString>
#include <QFile>
#include <QTextStream>

#include "global.h"

/**

	Uses rsync to generate a filtered list of files according to rsync's include/exclude rules
*/
class WRSYNC_API FileListGenerator : public QObject {
Q_OBJECT
public:
	FileListGenerator(QObject *parent=0);

signals:

public slots:

};

