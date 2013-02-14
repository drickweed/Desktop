#pragma once

/** QT headers **/
#include <QString>

/** JB headers **/
#include <jb/database/collection.h>

/** Application headers **/
#include "font.h"

using namespace JB;

namespace FontMate {

	class Fonts;

	class FontsDatabase : public RelationalCollection {
		RELATIONAL_COLLECTION(FontsDatabase);

		public:
			void updateFromPath(QString path);

			void updateFromModel(Fonts *fonts);

			RelationalObject *getObject() { return new Font; }
	};

}
