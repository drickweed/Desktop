#include "model/photo_selection.h"

/** QT headers **/
#include <QDebug>

/** Application headers **/
#include <storage/get.h>

namespace ImageImp {

	PhotoSelectionModel::PhotoSelectionModel(QObject *parent):
		Photos(parent) {
		setAlbumId(RelationalObject::invalidId());
		setPersonId(RelationalObject::invalidId());

		// Base query for showing available photoes for person
		query = QSqlQuery("select path from pictures where person_id = :person_id and album_id = :album_id");
	}

	PhotoSelectionModel::~PhotoSelectionModel() {
	}

	void PhotoSelectionModel::setDefaultPicture(const QModelIndex &index) {
		QSqlQuery query;

		// Delete old default photo
		query = QSqlQuery("delete from default_pictures where picture_id in (select id from pictures where album_id = :album_id and person_id = :person_id)");
		query.bindValue(":album_id", albumId());
		query.bindValue(":person_id", personId());
		query.exec();

		// Set new default photo
		query = QSqlQuery("insert into default_pictures (picture_id) values (:picture_id)");
		query.bindValue(":picture_id", photoId(images[index.row()]->path()));
		query.exec();

		// Invalidate model
		reset();
	}

	void PhotoSelectionModel::setAlbum(RelationalObjectRef album) {
		Photos::setAlbum(album);
		populate();
	}

	void PhotoSelectionModel::setPerson(RelationalObjectRef person) {
		Photos::setPerson(person);
		populate();
	}

	void PhotoSelectionModel::populate() {
		clear();

		query.bindValue(":person_id", personId());
		query.bindValue(":album_id", albumId());
		query.exec();

		while (query.next()) {
			possible_image_paths << query.record().value("path").toString();
		}

		Photos::populate();
	}

	void PhotoSelectionModel::reload() {
		populate();
	}

	QVariant PhotoSelectionModel::data(const QModelIndex & index, int role) const {
		if (not index.isValid())
			return QVariant();

		int record = index.row();
		Image *photo = images[record];
		QImage displayed_thumbnail;

		switch (role) {
			case Qt::DecorationRole:
				try {
					displayed_thumbnail = photo->thumbnail();
				} catch (ZeroDimentions *e) {
					return QVariant();
				}

				if (defaultPicture() == photoId(photo->path())) {
					QPixmap tagged_image = QPixmap::fromImage(displayed_thumbnail);
					if (JB::tagImage(&tagged_image, "default_photo", QPoint(3,3))) {
						return QVariant(tagged_image);
					}
				}

				return QVariant(displayed_thumbnail);
				break;
			case Qt::SizeHintRole:
				return QVariant(QSize(scale(), scale()));
			default:
				break;
		}

		return QVariant();
	}

	int PhotoSelectionModel::defaultPicture() const {
		return ImageImp::defaultPicture(albumId(),personId());
	}

	void PhotoSelectionModel::setAlbum(int album_id) {
		setAlbumId(album_id);
		populate();
	}

	void PhotoSelectionModel::setPerson(int person_id) {
		setPersonId(person_id);
		populate();
	}

}
