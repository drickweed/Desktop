#include "dictionary.h"

namespace Pdf {
	namespace Dictionary {
		QMap<QString, QString> parseFromData(QByteArray data) {
			QMap<QString, QString> dictionary;
			QString key, value;
			int index;

			// Remove dicrionary << and >> brackets
			if (data.startsWith("<<")) {
				data.remove(0, 2);
				data.remove(data.length()-2, 2);
			}

			while (not data.isEmpty()) {
				data = data.trimmed();

				if (data.at(0) == '/') // Skip first '/' if present
						data = data.remove(0, 1);

				if (data.isEmpty())
					break;

				index = data.indexOf(' ');
				if (key.isEmpty()) {
					key = QString(data.left(index)).trimmed();
					data.remove(0, index);
					continue;
				}

				index = data.indexOf('/', 0);
				if (index == -1) index = data.length();
				value = QString(data.left(index));
				data.remove(0, index);
				dictionary[key] = value.trimmed();
				key.clear();
			}
			return dictionary;
		}
	}
}
