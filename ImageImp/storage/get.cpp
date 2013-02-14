#include "get.h"

/** QT headers **/
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QString>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QDir>
#include <QFileInfo>

/** Application headers **/
#include "settings/system.h"
#include "imp/const.h"

using namespace JB;

namespace ImageImp {

	int photoId(const QString &path) {
		QSqlQuery query("select id from pictures where path = :path");
		query.bindValue(":path", path);
		query.exec();
		query.next();
		if (not query.isValid())
			return -1;
		return query.record().value("id").toInt();
	}

	QString personName(Image *photo) {

		QString person_name;
		QSqlQuery query;

		query.prepare("select people.name from pictures join people where pictures.person_id == people.id and path == :path");
		query.bindValue(":path", photo->path());
		query.exec();
		query.next();
		if (query.isValid()) {
			QSqlRecord record = query.record();
			person_name = record.value(0).toString();
		}

		return person_name;
	}

	int albumGroupId(int album_id) {
		int group_id = -1;

		QSqlQuery query("select group_id from albums where id == :album_id");
		query.bindValue(":album_id", album_id);
		query.exec();
		query.next();
		if (query.isValid()) {
			group_id = query.record().value(0).toInt();
		}
		return group_id;
	}

	QString templateNameFromPath(QString template_path) {
		return QFileInfo(template_path).baseName();
	}

	QString albumTemplate(QString path, QString table, int album_id) {
		QString template_name;

		QSqlQuery query(QString("select template from %1 where album_id == :album_id").arg(table));
		query.bindValue(":album_id", album_id);
		query.exec();
		query.next();
		if (query.isValid()) {
			template_name = query.record().value(0).toString();
		} else {
			QStringList template_files = templateFiles(path);
			if (not template_files.empty())
				template_name = QFileInfo(template_files[0]).baseName();
		}
		return template_name;
	}

	QString albumPictureTemplate(int album_id) {
		return albumTemplate(System::pictureTemplatesPath(), PICTURE_TEMPLATES_TABLE, album_id);
	}

	QString albumAlbumTemplate(int album_id) {
		return albumTemplate(System::albumTemplatesPath(), ALBUM_TEMPLATES_TABLE, album_id);
	}

	QStringList pictureTemplates() {
		return namesFromFiles(templateFiles(System::pictureTemplatesPath()));
	}

	QStringList albumTemplates() {
		return namesFromFiles(templateFiles(System::albumTemplatesPath()));
	}

	QStringList templateFiles(QString path) {
		QDir dir(path);
		dir.setFilter(QDir::Files);
		QStringList filters;
		filters << "*.psd";
		dir.setNameFilters(filters);
		return dir.entryList(QDir::Files, QDir::Name);
	}

	QStringList albumImagePaths(int album_id) {
		QStringList image_paths;

		QSqlQuery query("select path from pictures where album_id = :album_id");
		query.bindValue(":album_id", album_id);
		query.exec();

		while (query.next()) {
			QSqlRecord record = query.record();
			image_paths << record.value("path").toString();
		}
		return image_paths;
	}

	QStringList namesFromFiles(QStringList files) {
		QStringList names;
		foreach(QString file, files) {
			names << QFileInfo(file).baseName();
		}
		return names;
	}

	int defaultPicture(int album_id, int person_id) {
		QSqlQuery query("select picture_id from default_pictures where picture_id in (select id from pictures where album_id = :album_id and person_id = :person_id)");
		query.bindValue(":album_id", album_id);
		query.bindValue(":person_id", person_id);
		query.exec();
		query.next();
		if (not query.isValid())
			return -1;
		return query.record().value("picture_id").toInt();
	}

}
