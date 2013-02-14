#pragma once

/** QT headers **/
#include <QString>

namespace ImageImp {

	/**
		Implicit settings determined from the system and the location of the application.
	*/
	class System {
		public:

			/**
				Initialize implicit settings.
			*/
			static void init();

			/**
				Path to picture template files.
			*/
			static QString pictureTemplatesPath();

			/**
				Path to album template files.
			*/
			static QString albumTemplatesPath();

			/**
				Path to templates.
			*/
			static QString templatesPath();

			static QString exportPath();

			static QString scriptsPath();

			static QString translationFile();

			/**
				Root data path.
			*/
			static QString dataPath();

			static QString databasePath();

			static QString defaultPicturesPath();

			static bool rightToLeftLanguage();

		private:
			static QString m_data_path;

			System();
	};
}
