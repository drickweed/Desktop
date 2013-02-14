#pragma once

/** QT headers **/
#include <QObject>
#include <QList>
#include <QString>

/** Application headers **/
#include "target.h"
#include "translation.h"

class Document : public QObject {
	Q_OBJECT
	public:
		Document(QObject *parent=0);
};
