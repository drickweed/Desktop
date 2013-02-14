#include "picture.h"

/** JB headers **/
#include <jb/image/exif.h>

using namespace JB;

namespace ImageImp {

	Picture::Picture(QString path):
		RelationalObject(){
		setPath(path);
	}

	void Picture::setDefaults() {
		RelationalObject::setDefaults();
		setPath("New person");
	}

	QDateTime Picture::dateTime() const {
		return Exif(path()).dateTime();
	}

}
