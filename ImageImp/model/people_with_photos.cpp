#include "model/people_with_photos.h"

/** QT headers **/
#include <QDebug>
#include <QSqlError>

namespace ImageImp {

	void PeopleWithPhotos::init() {
		m_album_id = RelationalObject::invalidId();
		base_query = QSqlQuery("select people.id as id, people.name as name, people.sid as sid from pictures join people on people.id = pictures.person_id where album_id = :album_id group by people.id");
	}

	PeopleWithPhotos::~PeopleWithPhotos() {
	}

	void PeopleWithPhotos::setAlbum(RelationalObjectRef album_ref) {
		setAlbumId(album_ref.id());
		if (album_ref.isValid())
			updateQuery();
		else
			setQuery(QSqlQuery("false"));
	}

	void PeopleWithPhotos::updateQuery() {
		base_query.bindValue(":album_id", albumId());
		base_query.exec();
		setQuery(base_query);
		reset();
	}

	Qt::ItemFlags PeopleWithPhotos::flags(const QModelIndex& /* index */) const {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}
}
