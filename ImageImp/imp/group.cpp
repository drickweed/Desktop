#include "group.h"

namespace ImageImp {

	Group::Group(QString name):
		RelationalObject(){
		setName(name);
	}

	void Group::setDefaults() {
		RelationalObject::setDefaults();
		setName("New group");
	}

}


void ImageImp::Group::removeDependencies() {
	deleteRelated("people");
	deleteRelated("albums");
}
