#pragma once

/** QT headers **/
#include <QStringList>
#include <QByteArray>

namespace Pdf {
	namespace Grahpics {
		namespace Parse {

			/**
				Split pdf object reference array.
			*/
			QList<int> objectArray(QByteArray data);

			/**
				Split text node array.
			*/
			QStringList textArray(QByteArray data);

		}
	}
}
