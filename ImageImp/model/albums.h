#pragma once

/** JB headers **/
#include <jb/database/collection.h>

/** Application headers **/
#include "imp/album.h"

using namespace JB;

namespace ImageImp {

	/** Albums

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Albums : public RelationalCollection {
		Q_OBJECT
		RELATIONAL_COLLECTION(Albums);

		public:

			/** Table fields **/
			enum Fields {ID, NAME, GROUP_ID, LAST_IMAGE_PATH, NUMBER_OF_FIELDS};

			/** QT Api **/
			virtual bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

			/** JB Api **/
			RelationalObject* getObject() const { return new Album; }

	};

}
