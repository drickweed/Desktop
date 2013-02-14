#pragma once

/** QT headers **/
#include <QString>

namespace ImageImp {
	void setAlbumPictureTemplate(int album_id, QString template_name);

	void setAlbumAlbumTemplate(int album_id, QString template_name);

	void setAlbumTemplate(int album_id, QString table, QString template_name);

	void assignPicture(QString path, int album_id, int person_id);

	void unassignPicture(QString path);
}
