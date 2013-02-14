#pragma once

/** QT headers **/
#include <QString>
#include <QByteArray>
#include <QDateTime>

/** JB headers **/
#include <jb/database/object.h>

/** Application headers **/
#include "imp/group.h"
#include "imp/photodate.h"

using namespace JB;

namespace ImageImp {

	/**
		Picture.

		Belongs to a single person in a specific album.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Picture : public RelationalObject {
		Q_OBJECT
		RELATIONAL_OBJECT(Picture);

		SIMPLE_PROPERTY(QString, path, setPath, m_path);
		Q_PROPERTY(QString path READ path WRITE setPath)

		SIMPLE_PROPERTY(int, albumId, setAlbumId, m_album_id);
		Q_PROPERTY(int album_id READ albumId WRITE setAlbumId)
		BELONGS_TO(Album, album, setAlbum);

		SIMPLE_PROPERTY(int, personId, setPersonId, m_person_id);
		Q_PROPERTY(int person_id READ personId WRITE setPersonId)
		BELONGS_TO(Person, person, setPerson);

		public:
			Picture(QString path);

			void setDefaults();

			QString table() const { return "pictures"; }
			QString fieldName() const { return "picture"; }

			/**
				Date/time picture was taken.

				This information is taken from exif data in the image file.

				@return date/time picture was taken.
			*/
			QDateTime dateTime() const ;
	};
}
