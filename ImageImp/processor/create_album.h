#pragma once

/** QT headers **/
#include <QString>
#include <QMap>

/** JB headers **/
#include <jb/std/property.h>
#include <jb/database/object_ref.h>

/** STL headers **/
#include <exception>

using namespace JB;

namespace ImageImp {

	class UnsupportedPlatform : public std::exception {
	};

	class AlbumIsEmpty : public std::exception {
	};

	class NoAlbumSelected : public std::exception {
	};

	class AlbumCreator {
		SIMPLE_PROPERTY(RelationalObjectRef, album, setAlbum, m_album);

		public:

			AlbumCreator();

			void createAlbum();

			void exportAlbum();

			void collectAlbumDate();

			void sendToPhotoshop() const;

		private:
			/**
				Write file which contains location of exported data files.
			*/
			void writeExportHelperFile() const;

			void writeAlbumPersonPictureList() const;

			void writeAlbumSettings() const;

			void collectPicturesAndPeople();

			void collectExplicitPictureDefaults();

			QString albumExportDataPath() const;

			QString albumExportSettingsPath() const;

			QString albumExportHelperPath() const;

			bool albumHasData() const {
				return (not ordered_people.empty());
			}

			/**
				Path to visual base album import script.
			*/
			QString albumImportScriptPath() const;

			QString destinationAlbumFile() const;

			void fixDimentions(int &width, int &height, int picture_count) const;

			QMap<QString, QString> pictures_and_people;
			QList<QString> ordered_people;
	};

}
