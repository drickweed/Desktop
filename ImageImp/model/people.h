#pragma once

/** QT headers **/
#include <QList>

/** JB headers **/
#include <jb/database/collection.h>

/** Application headers **/
#include "imp/person.h"

using namespace JB;

namespace ImageImp {

	/** People

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class People : public RelationalCollection {
		Q_OBJECT
		RELATIONAL_COLLECTION(People);

		public:

			/** Database table fields **/
			enum Fields {ID, NAME, SID, GROUP_ID, NUMBER_OF_FIELDS};

			/** QT Api **/
			virtual bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

			/** JB Api **/
			RelationalObject* getObject() const { return new Person; }

	};

}
