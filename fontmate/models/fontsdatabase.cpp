#include "fontsdatabase.h"

/** Application headers **/
#include "fonts.h"

namespace FontMate {

	void FontsDatabase::updateFromPath(QString /* path */) {

	}

	void FontsDatabase::updateFromModel(Fonts *fonts) {
		for (int i=0; i<fonts->rowCount(); i++) {

		}
	}

}
