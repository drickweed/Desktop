#include "model/photo_import.h"

/** QT headers **/
#include <QDir>
#include <QSize>
#include <QtConcurrentMap>
#include <QSqlError>
#include <QBrush>
#include <QColor>

/** JB headers **/
#include <jb/qt4/pathutils.h>
#include <jb/image/exif.h>
#include <jb/std/log.h>

/** Application headers **/
#include <storage/get.h>
#include <storage/set.h>
#include <storage/query.h>

using namespace JB;

namespace ImageImp {

	QVariant PhotoImportModel::data(const QModelIndex & index, int role) const {
		if (not index.isValid())
			return QVariant();

		int image_row = index.row();
		Image *photo = images[image_row];

		if (not photo)
			return QVariant();

		QString name = personName(photo);

		switch (role) {
			case Qt::DisplayRole:
				if (not name.isEmpty())
					return QVariant(name);
				else
					return QVariant(tr("New photograph"));
				break;
			case Qt::ToolTipRole:
				try {
					return QVariant(Exif(photo->path()).dateTime());
				} catch (JB::DateTimeParseFailure *e) {
					return QVariant(tr("No Date/Time"));
				}
				break;
			case Qt::DecorationRole:
				try {
					return QVariant(photo->thumbnail());
				} catch (ZeroDimentions *e) {
				}
				break;
			case Qt::SizeHintRole:
				return QVariant(QSize(scale()+6, scale()+6));
				break;
			case Qt::BackgroundRole:
				if (name.isEmpty())
					return QVariant(QBrush(QColor(0,150, 0)));
				break;
			default:
				break;
		}

		return QVariant();
	}

	void PhotoImportModel::populate(const QString &path) {

		// Currently loaded path re-selected => ignore
		if (QDir(this->path()) == QDir(path))
			return;

		clear();

		setPath(path);

		// Validate path
		QFileInfo path_info(path);
		if ((path.isEmpty()) or (not path_info.isDir()))
			return;

		// Collect files from selected path
		QDir dir(path);
		foreach(QString file, dir.entryList(QDir::Files)) {
			possible_image_paths << Path::join(path, file);
		}

		Photos::populate();
	}

	void PhotoImportModel::assignPerson(const QModelIndex &index) {
		if ((not hasPerson()) or (not hasAlbum()) or (not index.isValid()))
			return;

		unassignPerson(index);
		assignPicture(images[index.row()]->path(), albumId(), personId());

		emit dataChanged(index, index);
		emit albumPhotoAssignmentChanged(albumId());
	}

	void PhotoImportModel::unassignPerson(const QModelIndex &index) {
		if (not index.isValid())
			return;

		unassignPicture(images[index.row()]->path());

		emit dataChanged(index, index);
		emit albumPhotoAssignmentChanged(albumId());
	}

	int PhotoImportModel::calculatedTimeDifference() {
		int time_difference = 0;

		LOG("Calculating time difference between first picture and person");

		if (empty())
			return 0;

		RelationalObjects photo_dates = sortedPhotoDates();

		if (photo_dates.empty())
			return 0;

		LOG("First person date " << PhotoDate(photo_dates[0]).dateTime().toString().toStdString())

		QStringList sorted_pictures = dateSortedPictures();

		if (sorted_pictures.empty())
			return 0;

		LOG("First picture date " << Exif(sorted_pictures[0]).dateTime().toString().toStdString())

		time_difference = Exif(sorted_pictures[0]).dateTime().addSecs(time_difference).secsTo(PhotoDate(photo_dates[0]).dateTime());

		LOG("Time difference calculated " << time_difference);

		return time_difference;
	}

	void PhotoImportModel::autoAssign(int time_difference) {

		if (empty())
			return;

		RelationalObjects photo_dates = sortedPhotoDates();

		if (photo_dates.empty())
			return;

		QStringList sorted_pictures = dateSortedPictures();

		if (sorted_pictures.empty())
			return;

		// Iterators
		RelationalObjects::Iterator photo_date = photo_dates.begin();
		QStringList::Iterator picture = sorted_pictures.begin();

		// Only one person. assign all pictures to that person
		if (photo_dates.size() == 1) {
			for (; picture != sorted_pictures.end(); picture++)
				assignPerson(PhotoDate(*photo_date).person(), pathIndex(*picture));
			return;
		}

		RelationalObjects::iterator next_photo_date = photo_date + 1;

		// More than one person.
		// Loop as long as there are 2 people to compare pictures with
		while (next_photo_date != photo_dates.end() && picture != sorted_pictures.end()) {
			if (Exif(*picture).dateTime().addSecs(time_difference) <= PhotoDate(*next_photo_date).dateTime()) {
				assignPerson(PhotoDate(*photo_date).person(), pathIndex(*picture));
				picture++;
			}
			else {
				photo_date++;
				next_photo_date++;
			}
		}

		// Assign the rest of the pictures to the last person
		for(;picture!=sorted_pictures.end(); picture++)
			assignPerson(PhotoDate(*photo_date).person(), pathIndex(*picture));
	}

	PhotoImportModel::~PhotoImportModel() {
	}

	PhotoImportModel::PhotoImportModel(QObject *parent):
		Photos(parent) {
		setPopulateMethod(PopulateReplace);

		connect(this, SIGNAL(modelReset()), SIGNAL(modelChanged()));
		connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), SIGNAL(modelChanged()));
		connect(this, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), SIGNAL(modelChanged()));
		connect(this, SIGNAL(rowsInserted(const QModelIndex&, int, int)), SIGNAL(modelChanged()));
	}

	void PhotoImportModel::assignPerson(const RelationalObjectRef person, const QModelIndex & index) {

		if ((not hasAlbum()) or (not index.isValid()))
			return;

		unassignPerson(index);
		assignPicture(images[index.row()]->path(), albumId(), Person(person).id());

		emit dataChanged(index, index);
		emit albumPhotoAssignmentChanged(albumId());
	}

	void PhotoImportModel::populateFromDatabase() {
		if (not hasAlbum())
			return;

		if (populateMethod() == PopulateReplace)
			clear();

		possible_image_paths = Album(albumId()).pictures();
		Photos::populate();
	}

	RelationalObjects PhotoImportModel::sortedPhotoDates() const {

		Album album;
		album.load(albumId());
		PhotoDate photodate;
		RelationalObjects photo_dates = album.manyRelated(&photodate);

		if (photo_dates.empty())
			return photo_dates;

		// This is not as trivial as it seems. moving to the map is used to sort by
		// the photodate's date/time.
		QMap<QDateTime, RelationalObjectRef> photo_dates_map;
		foreach(RelationalObjectRef photo_date, photo_dates)
			photo_dates_map.insert(PhotoDate(photo_date).dateTime(), photo_date);

		// This is actually a sorted version of the previous list.
		return photo_dates_map.values();
	}

	QStringList PhotoImportModel::dateSortedPictures() const {
		// Pictures
		QMap<QDateTime, QString> sorted_pictures_map;
		foreach(QString path, loaded_image_paths) {
			try {
				sorted_pictures_map.insert(Exif(path).dateTime(), path);
			} catch (JB::DateTimeParseFailure *e) {
				// Ignore pictures with missing dates.
			}
		}

		 return sorted_pictures_map.values();
	}

}
