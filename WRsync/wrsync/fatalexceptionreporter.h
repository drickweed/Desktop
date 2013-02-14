#pragma once

/** QT headers **/
#include <QObject>

class FatalExceptionReporter : public QObject {
	Q_OBJECT
	public:
		explicit FatalExceptionReporter(QObject *parent = 0);

	signals:

	public slots:

};
