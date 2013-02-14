#pragma once

/** Application headers **/
#include "model/photos.h"

/** JB headers **/
#include <jb/std/property.h>

using namespace JB;

namespace ImageImp {

	/**
		Listing of pictures to assign to people in a group.
	*/
	class PhotoImportModel : public Photos {
		Q_OBJECT

		enum PopulationMethod {PopulateAppend, PopulateReplace};

		SIMPLE_PROPERTY(PopulationMethod, populateMethod, setPopulateMethod, m_populate_method);

		public:
			PhotoImportModel(QObject *parent=0);

			virtual ~PhotoImportModel();

			/** QT Api **/
			virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

			/**
				Calculate time difference between first person in album and oldest picture.

				@return Time difference. If any of the lists don't have any items, returns 0.
			*/
			int calculatedTimeDifference();

			RelationalObjects sortedPhotoDates() const;

			QStringList dateSortedPictures() const;

		public slots:

			/**
				Populate model with list of images from given path.

				@param path path to get image files from
			*/
			void populate(const QString &path);

			/**
				Populate the model from images already set in the database.
			*/
			void populateFromDatabase();

			/**
				Assign selected person to image.

				@param index index of photo to assign to person
			*/
			void assignPerson(const QModelIndex &index);

			/**
				Unassign person from image.

				@param index index of photo to unassign from person.
			*/
			void unassignPerson(const QModelIndex &index);

			/**
				Automatically assign pictures to the people in the given album.

				@param time_difference time difference in seconds between time on camera and time on computer. Negative if time on computer is earlier.

				@internal this is done according to datetime stamps in the album and on the exif
				information in the image files. If no exif information exists, it is done on the file creation date.
			*/
			void autoAssign(int time_difference=0);

		signals:
			void albumPhotoAssignmentChanged(int album_id);
			void modelChanged();

		private:
			/**
				Assign picture to person in currently set album.

				@param person person to assign to picture.
				@param index index of picture to assign person to.
			*/
			void assignPerson(const RelationalObjectRef person, const QModelIndex &index);

	};
}

