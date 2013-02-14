#include "clean_db.h"

/** QT headers **/
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFileInfo>
#include <QVariant>

namespace ImageImp {

	void DatabaseCleaner::removeMissingPictures() {
		QList<int> missing_picture_ids = missingPictureIds();
		emit processingStarted(missing_picture_ids.count()-1);
		int i=0;

		foreach(int id, missing_picture_ids) {
			QSqlQuery query("remove from pictures where id = :id");
			query.bindValue(":id", id);
			query.exec();

			emit step(++i);
		}
	}

	void DatabaseCleaner::clean() {

		removeMissingPictures();
	}

	QList<int> DatabaseCleaner::missingPictureIds() {
		QList<int> missing_picture_ids;

		QSqlQuery query("select id, path from pictures");
		QSqlRecord record;

		emit processingStarted(query.size()-1);
		int i=0;

		while (query.next()) {
			record = query.record();
			QFileInfo file(record.value("path").toString());
			if (not file.exists())
				missing_picture_ids.append(record.value("id").toInt());
			emit step(++i);
		}
		return missing_picture_ids;
	}

	DatabaseCleaner::DatabaseCleaner():
		QObject(NULL) {
	}

}
