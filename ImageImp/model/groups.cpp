#include "groups.h"

/** JB headers **/
#include <jb/std/memory.h>
#include <jb/qt4/stringutils.h>

namespace ImageImp {

	bool Groups::setData(const QModelIndex &index, const QVariant &value, int role) {
		if (not index.isValid())
			return false;

		int field = index.column();

		if (role == Qt::EditRole)
			switch (field) {
				case NAME:
					// Name cannot be empty. revert to previous value.
					if (value.toString().isEmpty())
						return false;
				default:
					break;
			}

		return RelationalCollection::setData(index, value, role);
	}

}
