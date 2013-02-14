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
	class PeopleWithPhotosQuery: public QSqlQueryModel {
		Q_OBJECT

		SIMPLE_PROPERTY(int, albumId, setAlbumId, m_album_id);

		public:

			PeopleWithPhotosQuery(QObject *parent=0);
			virtual ~PeopleWithPhotosQuery();

			/** QT Api **/
			virtual Qt::ItemFlags flags(const QModelIndex &index) const;

			/**
				QT Api

				This implementation capitalizes the first letter of the header text.
			*/
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

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
