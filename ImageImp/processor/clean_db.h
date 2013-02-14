#pragma once

/** QT headers **/
#include <QList>
#include <QObject>

/** JB headers **/
#include <jb/std/global.h>

namespace ImageImp {

	/**
		Database cleaner.
	*/
	class DatabaseCleaner : public QObject {
		Q_OBJECT

		public:
			DatabaseCleaner();

			void clean();

		private:
			void removeMissingPictures();

			QList<int> missingPictureIds();

		signals:
			void processingStarted(int steps);

			void step(int step);

	};

}
