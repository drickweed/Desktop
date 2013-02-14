#pragma once

/** JB headers **/
#include <jb/database/collection.h>

/** Application headers **/
#include "imp/group.h"

using namespace JB;

namespace ImageImp {

	/** Groups

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Groups : public RelationalCollection {
		Q_OBJECT
		RELATIONAL_COLLECTION(Groups);

		public:

			enum Fields {ID, NAME, PARENT_ID, NUMBER_OF_FIELDS};

			/** QT Api **/
			virtual bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

			/** JB Api **/
			RelationalObject* getObject() const { return new Group; }

	};

}
