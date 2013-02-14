#pragma once

/** QT headers **/
#include <QString>
#include <QDateTime>

/** JB headers **/
#include <jb/database/object.h>

/** Application headers **/
#include "imp/person.h"
#include "imp/album.h"

using namespace JB;

namespace ImageImp {

	/**
		PhotoDate.

		A Photo Date represents the date/time at which the pictures of a specific person were taken. This data can later be used to automatically assign photos to this person.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class PhotoDate : public RelationalObject {
		Q_OBJECT
		RELATIONAL_OBJECT(PhotoDate);

		SIMPLE_PROPERTY(QDateTime, dateTime, setDateTime, m_date_time);
		Q_PROPERTY(QDateTime datetime READ dateTime WRITE setDateTime)

		SIMPLE_PROPERTY(int, album_id, setAlbumId, m_album_id);
		Q_PROPERTY(int album_id READ album_id WRITE setAlbumId)
		BELONGS_TO(Album, album, setAlbum);

		SIMPLE_PROPERTY(int, person_id, setPersonId, m_person_id);
		Q_PROPERTY(int person_id READ person_id WRITE setPersonId)
		BELONGS_TO(Person, person, setPerson);

		public:
			PhotoDate(Person *person);

			QString table() const { return "photodates"; }
			QString fieldName() const { return "photodate"; }
	};
}
