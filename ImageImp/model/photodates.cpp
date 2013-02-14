#include "model/photodates.h"

#include <QSqlRelation>

#include <jb/std/memory.h>
#include <jb/qt4/stringutils.h>

namespace ImageImp {

	bool PhotographedPeopleModel::albumSelected() {
		return hasFilters();
	}

	QVariant PhotographedPeopleModel::headerData(int section, Qt::Orientation orientation, int role) const {

		// Capitalize default header names
		switch (role) {
			case Qt::DisplayRole:
				if ((section == PhotographedPeopleModel::PERSON_ID) and (orientation == Qt::Horizontal))
					return QVariant(tr("Name"));
				break;
			default:
				break;
		}

		return RelationalCollection::headerData(section, orientation, role);
	}
}
