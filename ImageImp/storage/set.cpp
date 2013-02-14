#include "set.h"

/** Application headers **/
#include "get.h"

/** QT headers **/
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QVariant>

namespace ImageImp {

	void setAlbumTemplate(int album_id, QString table, QString template_name) {
		QString album_template = albumAlbumTemplate(album_id);
		QSqlQuery query(QString("replace into %1 values(:album_id, :template)").arg(table));
		query.bindValue(":album_id", album_id);
		query.bindValue(":template", template_name);
		query.exec();
	}

	void setAlbumPictureTemplate(int album_id, QString template_name) {
		setAlbumTemplate(album_id, "picture_templates", template_name);
	}

	void setAlbumAlbumTemplate(int album_id, QString template_name) {
		setAlbumTemplate(album_id, "album_templates", template_name);
	}

	void assignPicture(QString path, int album_id, int person_id) {
		QSqlQuery query;
		query.prepare("insert into pictures (path, album_id, person_id) values (:path, :album_id, :person_id)");
		query.bindValue(":path", path);
		query.bindValue(":album_id", album_id);
		query.bindValue(":person_id", person_id);
		query.exec();
	}

	void unassignPicture(QString path) {
		QSqlQuery query;
		query.prepare("delete from pictures where path = :picture");
		query.bindValue(":picture", path);
		query.exec();
	}

}
