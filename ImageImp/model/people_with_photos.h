#pragma once

/** QT headers **/
#include <QSqlQueryModel>

/** JB headers **/
#include <jb/std/memory.h>
#include <jb/std/property.h>
#include <jb/database/collection.h>

/** Application headers **/
#include "imp/album.h"
#include "imp/person.h"

using namespace JB;

namespace ImageImp {

	/**

	*/
	class PeopleWithPhotos: public RelationalCollection {
		Q_OBJECT
		RELATIONAL_COLLECTION(PeopleWithPhotos);

		SIMPLE_PROPERTY(int, albumId, setAlbumId, m_album_id);

		public:

			virtual ~PeopleWithPhotos();

			/** QT Api **/
			virtual Qt::ItemFlags flags(const QModelIndex &index) const;

			/** JB Api **/
			RelationalObject* getObject() const { return new Person; }
			virtual void init();

		public slots:

			/**
				Set album id from given album object.

				@param album album to filter list of people with photos with.
			*/
			void setAlbum(RelationalObjectRef album_ref);

			/**
				Update sql query representing the model.
			*/
			void updateQuery();

		private:

			QSqlQuery base_query;
	};

}
