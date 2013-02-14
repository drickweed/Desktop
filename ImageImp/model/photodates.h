#pragma once

/** QT headers **/
#include <QList>

/** JB headers **/
#include <jb/database/collection.h>

/** Application headers **/
#include "imp/photodate.h"
#include "imp/person.h"

using namespace JB;

namespace ImageImp {

	/** People

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class PhotographedPeopleModel : public RelationalCollection {
		Q_OBJECT
		RELATIONAL_COLLECTION(PhotographedPeopleModel);

		public:

			/** Table fields **/
			enum Fields {ID, DATETIME, ALBUM_ID, PERSON_ID, NUMBER_OF_FIELDS};

			/** JB Api **/
			RelationalObject* getObject() const { return new PhotoDate; }

			bool albumSelected();

			/** QT Api */
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;


	};

}
