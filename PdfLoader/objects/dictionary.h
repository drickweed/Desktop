#pragma once

/** QT headers **/
#include <QMap>
#include <QString>
#include <QByteArray>

namespace Pdf {
	namespace Dictionary {
		QMap<QString, QString> parseFromData(QByteArray data);
	}
}
