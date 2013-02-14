#pragma once

#include <QObject>

#include "global.h"

class WRSYNC_API RemoteComputer : public QObject
{
Q_OBJECT
public:
	explicit RemoteComputer(QObject *parent = 0);

signals:

public slots:

};
