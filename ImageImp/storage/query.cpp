#include "query.h"

/** QT headers **/

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QString>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QDir>
#include <QFileInfo>


bool ImageImp::hasPhotoDate(RelationalObjectRef album, RelationalObjectRef person) {
	QSqlQuery query("select * from photodates where album_id = :album_id and person_id = :person_id");
	query.bindValue(":album_id", album.id());
	query.bindValue(":person_id", person.id());
	query.exec();
	return query.size() > 0;
}
