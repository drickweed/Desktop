#pragma once

/** QT headers **/
#include <QString>

/** JB headers **/
#include <jb/image/image.h>

using namespace JB;

namespace ImageImp {

	/**
		Get ID for the photo if it is in the database.

		@param path image to look for ID for
		@return Image ID in database. -1 if image not in database
	*/
	int photoId(const QString &path);

	/**
		Get name or person assigned to photo.

		@param photo photo to get assigned person name for
		@return person name if assigned to photo. Empty string if no person assigned.
	*/
	QString personName(Image *photo);

	/**
		Get group ID for given album.

		@param album_id album id to get group for.
		@return group id for given album. -1 if invalid album id given.
	*/
	int albumGroupId(int album_id);


	/**
		Get images for given album.

		@return Image list with full paths.
	*/
	QStringList albumImagePaths(int album_id);

	/**
		Get picture template for given album
	*/
	QString albumPictureTemplate(int album_id);

	/**
		Get album template for given album
	*/
	QString albumAlbumTemplate(int album_id);

	/**
		Available picture template names.
	*/
	QStringList pictureTemplates();

	/**
		Available album template names.
	*/
	QStringList albumTemplates();

	/**
		Get template file names from path.

		@param path path to check for template files.
	*/
	QStringList templateFiles(QString path);

	/**
		Get names from file names.

		@return basename of each file.
	*/
	QStringList namesFromFiles(QStringList files);

	/**
		Get default picture for album/person.

		@param album_id Album ID.
		@param person_id Person ID.
		@return Picture id. -1 if no explicit default picture set.
	*/
	int defaultPicture(int album_id, int person_id);

}
