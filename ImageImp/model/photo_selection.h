#pragma once

#include "model/photos.h"

#include "imp/person.h"
#include "imp/album.h"

namespace ImageImp {

	class PhotoSelectionModel : public Photos {
		Q_OBJECT

		public:
			PhotoSelectionModel(QObject *parent=0);
			~PhotoSelectionModel();

			/** QT Api **/
			virtual QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;

			virtual void populate();

		public slots:

			void setPerson(RelationalObjectRef person);

			void setAlbum(RelationalObjectRef album);

			void setAlbum(int album_id);

			void setPerson(int person_id);

			/**
				Set default picture for selected person in album

				@param index index in model for the default photo
			*/
			void setDefaultPicture(const QModelIndex &index);

			/**
				Get default picture for selected person

				@return default picture id in pictures table
			*/
			int defaultPicture() const;

			void reload();

		private: /** Properties **/
			QSqlQuery query;

	};
}
