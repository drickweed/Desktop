#pragma once

#include <QSortFilterProxyModel>
#include <QString>

#include "imp/album.h"
#include "imp/person.h"


namespace ImageImp {

	/**
		Filter for people who can be added to the current album.

		Filter by:
		- Text search on name and person id
		- Filter out people who were already added to the album.

	*/

	class UnphotographedPeopleProxyModel : public QSortFilterProxyModel {
		Q_OBJECT

		public:
			UnphotographedPeopleProxyModel(QObject *parent);

		public slots:

			/**
				Set the album to show people from.

				People who already have a photo in the album will not be shown.

				@param album the album to show people from.
			*/
			void setAlbumFilter(RelationalObjectRef album);

		protected:
			/** Qt API **/
			bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
			bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent ) const;
			Qt::ItemFlags flags(const QModelIndex &index) const;
			/**
				Check if a person has been assigned a date/time in the album.
			*/
			bool personInAlbum(RelationalObject *person) const;
			bool personInAlbum(int person_id) const;

		private:

			QString filter;

			int album_id;

	};

}
