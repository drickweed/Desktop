#include "unphotographed_people.h"

#include <QDebug>

#include <jb/database/collection.h>
#include <jb/std/memory.h>

#include "model/people.h"

namespace ImageImp {

	UnphotographedPeopleProxyModel::UnphotographedPeopleProxyModel(QObject *parent):
		QSortFilterProxyModel(parent) {
		setFilterCaseSensitivity(Qt::CaseInsensitive);
	}

	bool UnphotographedPeopleProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {



		// Text serach
		QModelIndex nameIndex = sourceModel()->index(source_row, 1, source_parent);
		QModelIndex sidIndex = sourceModel()->index(source_row, 2, source_parent);

		bool match_search = sourceModel()->data(nameIndex).toString().contains(filterRegExp()) || sourceModel()->data(sidIndex).toString().contains(filterRegExp());

		// Do not list people with photos
		RelationalCollection *r_model = dynamic_cast<RelationalCollection*>(sourceModel());
		RelationalObject *person = r_model->getObject(source_row);

		bool person_has_photo_taken = personInAlbum(person);

		DELETE(person);

		return ((not person_has_photo_taken) && (match_search));
	}

	void UnphotographedPeopleProxyModel::setAlbumFilter(RelationalObjectRef album_ref) {
		album_id = album_ref.id();
		if (album_ref.isValid())
			filter = QString("%1 == %2").arg(getIdField(album_ref.fieldName())).arg(album_ref.id());
		else
			filter = "false";

		invalidateFilter();
	}

	bool UnphotographedPeopleProxyModel::personInAlbum(int person_id) const {
		QString query_string = QString("select id from photodates where %1 and person_id == %2").arg(filter).arg(person_id);

// 		qDebug() << "Query :" << query_string;

		QSqlQuery query(query_string);
// 		query.exec();
		query.next();

		bool result = query.isValid();

// 		if (result)
// 			qDebug() << "Person" << person_id << "is in album" << filter;
// 		else
// 			qDebug() << "Person" << person_id << "is not in album" << filter;

		return result;
	}

	bool UnphotographedPeopleProxyModel::personInAlbum(RelationalObject *person) const {
		return personInAlbum(person->id());
	}

	bool UnphotographedPeopleProxyModel::filterAcceptsColumn(int source_column, const QModelIndex& /* source_parent */) const {

		// Show only NAME and SID
		switch (source_column) {
			case People::NAME:
			case People::SID:
				return true;
			default:
				break;
		}
		return false;
	}

}

Qt::ItemFlags ImageImp::UnphotographedPeopleProxyModel::flags(const QModelIndex& /* index */) const {
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
