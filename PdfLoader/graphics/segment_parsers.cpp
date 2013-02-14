#include "segment_parsers.h"

#include <QVariant>

namespace Pdf {
	namespace Grahpics {
		namespace Parse {

			QList<int> objectArray(QByteArray data) {
				QList<QByteArray> objects_data;
				objects_data = data.split('R');
				for (int i=0; i<objects_data.count(); i++) {
					if (QString(objects_data[i]).trimmed().isEmpty()) {
						objects_data.removeAt(i);
						i--;
					}
				}

				QList<int> objects;
				bool conversion_ok;
				QString object_id_segment;
				foreach (QString object, objects_data) {
					object_id_segment = object.split(' ')[0];
					int object_id = QVariant(object_id_segment).toInt(&conversion_ok);
					if (conversion_ok)
						objects.append(object_id);
				}
				return objects;
			}

			QStringList textArray(QByteArray data) {
				QStringList text_nodes;
				return text_nodes;
			}
		}
	}
}
