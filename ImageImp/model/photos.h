#pragma once

/** QT headers **/
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QList>

/** Application headers **/
#include "imp/photodate.h"
#include "imp/person.h"
#include "imp/album.h"

/** JB headers **/
#include <jb/std/property.h>
#include <jb/image/imagesmodel.h>

using namespace JB;

namespace ImageImp {

	/**
		Base model for handling photos.
	*/
	class Photos : public ImagesModel {
		Q_OBJECT

		SIMPLE_PROPERTY(int, albumId, setAlbumId, m_album_id);
		SIMPLE_PROPERTY(int, personId, setPersonId, m_person_id);

		public:
			Photos(QObject *parent=0);

			/**
				Check if model has an album assigned.
			*/
			bool hasAlbum() const { return albumId() != RelationalObject::invalidId(); }

			/**
				Check if model has a peron assigned.
			*/
			bool hasPerson() const { return personId() != RelationalObject::invalidId(); }

		public slots:

			/**
				Set the person to be assigned to select photos.

				The model DOES NOT take ownership of the person.

				@internal person is assigned when a picture is double-clicked.

				@param person person to assign to selected photos.
			*/
			void setPerson(RelationalObjectRef person);

			/**
				Set the album the images will belong to.

				The model DOES NOT take ownership of the album.

				@param album the album the images will belong to.

			*/
			void setAlbum(RelationalObjectRef album);

	};

}
