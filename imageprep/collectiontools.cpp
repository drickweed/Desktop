#include "collectiontools.h"
#include <QDir>

namespace CollectionTools {
	int dirImageCount(QString path) {
		QDir dir(path, "*.png;*.jpg", QDir::Name, QDir::Files);
		return dir.count();
	}
}
