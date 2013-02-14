#pragma once

#include <QSortFilterProxyModel>
#include <QString>

#include "imp/album.h"
#include "imp/person.h"


namespace ImageImp {

	/**
		Filter for people who have photos assigned to them in the selected album.
	*/

	class PhotographedPeopleProxyModel : public QSortFilterProxyModel {
		Q_OBJECT

		public:
			PhotographedPeopleProxyModel(QObject *parent);

		public slots:

			/**
				Set the album to show people with photos in the album

				@param album the album to show people from.
			*/
			void setAlbumFilter(RelationalObjectRef album);

		protected:
			/** Qt API **/
			bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
			bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent ) const;

			/**
				Check if a person has photos in the album
			*/
			bool personWithPhotos(RelationalObject *person) const;
			bool personWithPhotos(int person_id) const;

		private:

			QString filter;

			int album_id;

	};

}
