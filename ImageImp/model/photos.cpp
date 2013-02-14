#include "model/photos.h"

/** QT headers **/
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QtConcurrentMap>
#include <QDebug>


#include <exception>

/** JB headers **/
#include <jb/qt4/pathutils.h>
#include <jb/std/memory.h>

/** Application headers **/
#include <imp/const.h>
#include <storage/get.cpp>

using namespace JB;

namespace ImageImp {

	Photos::Photos(QObject * parent):
		ImagesModel(parent),
		m_album_id(RelationalObject::invalidId()),
		m_person_id(RelationalObject::invalidId()) {

		setScale(IMAGEIMP_MIN_THUMBNAIL_SCALE);
	}

	void Photos::setPerson(RelationalObjectRef person_ref) {
		setPersonId(person_ref.id());
	}

	void Photos::setAlbum(RelationalObjectRef album_ref) {
		setAlbumId(album_ref.id());
	}

}
