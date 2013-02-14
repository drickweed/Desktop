#include "model/people_with_photos_query.h"

/** QT headers **/
#include <QDebug>
#include <QSqlError>

/** JB headers **/
#include <jb/qt4/stringutils.h>

namespace ImageImp {

	PeopleWithPhotosQuery::PeopleWithPhotosQuery(QObject *parent):
		QSqlQueryModel(parent) {
		m_album_id = RelationalObject::invalidId();
		base_query = QSqlQuery("select people.id as id, people.name as name, people.sid as sid from pictures join people on people.id = pictures.person_id where album_id = :album_id group by people.id");
	}

	PeopleWithPhotosQuery::~PeopleWithPhotosQuery() {
	}

	QVariant PeopleWithPhotosQuery::headerData(int section, Qt::Orientation orientation, int role) const {

		switch (role) {
			case Qt::DisplayRole:
				return QVariant(QObject::tr(capitalize(QSqlQueryModel::headerData(section, orientation, role).toString()).toStdString().c_str()));
			default:
				break;
		}

		return QSqlQueryModel::headerData(section, orientation, role);
	}

	void PeopleWithPhotosQuery::setAlbum(RelationalObjectRef album_ref) {
		setAlbumId(album_ref.id());
		if (album_ref.isValid())
			updateQuery();
		else
			clear();
	}

	void PeopleWithPhotosQuery::updateQuery() {
		base_query.bindValue(":album_id", albumId());
		base_query.exec();
		setQuery(base_query);
		reset();
	}

	Qt::ItemFlags PeopleWithPhotosQuery::flags(const QModelIndex& /* index */) const {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}
}
