#include "document.h"

/** QT headers **/
#include <QSqlRelation>

#include <jb/std/log.h>

/** Application headers **/
#include "settings/system.h"
#include "imp/const.h"

namespace ImageImp {

	Document::Document(QObject *parent):
		QObject(parent),
		groups(NULL),
		people(NULL),
		albums(NULL),
		photo_import(NULL),
		person_photos(NULL),
		people_with_photos(NULL),
		photographed_people(NULL) {

		filesystemWatchers();
		setupModels();
		setupProxyModels();
		connectModels();
	}

	void Document::setupModels() {

		groups = new Groups(this);
		people = new People(this);
		people->setDisplayMode(CollectionDisplay::Filtered);
		albums = new Albums(this);
		albums->setDisplayMode(CollectionDisplay::Filtered);

		photographed_people = new PhotographedPeopleModel(this);
		photographed_people->setReadOnly(true);

		photo_import = new PhotoImportModel(this);
		people_with_photos = new PeopleWithPhotos(this);
		people_with_photos_query = new PeopleWithPhotosQuery(this);
		person_photos = new PhotoSelectionModel(this);

		picture_templates = new TemplatesModel(System::pictureTemplatesPath(), Template::PictureTemplate, this);
		album_templates = new TemplatesModel(System::albumTemplatesPath(), Template::AlbumTemplate, this);

	}

	void Document::setupProxyModels() {

		// Ensure all models are setup
		Q_ASSERT(groups);
		Q_ASSERT(people);
		Q_ASSERT(albums);
		Q_ASSERT(photographed_people);
		Q_ASSERT(photo_import);
		Q_ASSERT(people_with_photos);
		Q_ASSERT(person_photos);
		Q_ASSERT(picture_templates);
		Q_ASSERT(album_templates);

		// Group search proxy
		groups_search_proxy = new JB::SearchProxyModel(this);
		groups_search_proxy->setSourceModel(groups);
		groups_search_proxy->setSearchColumns(QList<int>() << 1);

		// People search proxy
		people_search_proxy = new JB::SearchProxyModel(this);
		people_search_proxy->setSourceModel(people);
		people_search_proxy->setSearchColumns(QList<int>() << 1);

		// Album search proxy
		albums_search_proxy = new JB::SearchProxyModel(this);
		albums_search_proxy->setSourceModel(albums);
		albums_search_proxy->setSearchColumns(QList<int>() << 1);

		// Search proxy for people list in photo importing view
		photo_import_people_search_proxy = new JB::SearchProxyModel(this);
		photo_import_people_search_proxy->setSourceModel(people);
		photo_import_people_search_proxy->setSearchColumns(QList<int>() << 1);

		// Filter/search proxy for list of unphotographed people in photograph people view
		unphotographed_people = new UnphotographedPeopleProxyModel(this);
		unphotographed_people->setSourceModel(people);

		ordered_people_proxy = new StoredOrderProxyModel(this);
		ordered_people_proxy->setOrderTable("ordered_people");
		ordered_people_proxy->setExclusionTable("excluded_people");
		ordered_people_proxy->setExclusionColumn(1);
		ordered_people_proxy->setSourceModel(people_with_photos_query);

		photographed_people_names = new RelationalProxyModel(this);
		photographed_people_names->setSourceModel(photographed_people);
		photographed_people_names->setRelation(PhotographedPeopleModel::PERSON_ID, QSqlRelation("people","id","name"));
	}

	void Document::reload() {
		groups->reload();
		people->reload();
		albums->reload();
		photographed_people->reload();
		people_with_photos->updateQuery();
		people_with_photos_query->updateQuery();
	}

	void Document::setAlbum(RelationalObjectRef album) {
		m_album = album;
		photographed_people->setRelationFilter(album);
		unphotographed_people->setAlbumFilter(album);
		photo_import->setAlbum(album);
		people_with_photos->setAlbum(album);
		people_with_photos_query->setAlbum(album);
		person_photos->setAlbum(album);
		picture_templates->setAlbum(album);
		album_templates->setAlbum(album);
	}

	void Document::setGroup(RelationalObjectRef group) {
		people->setRelationFilter(group);
		albums->setRelationFilter(group);
	}

	void Document::connectModels() {
		LOG("Establishing internal connections in document");

		connect(people, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), people_with_photos, SLOT(updateQuery()));
		connect(people, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), people_with_photos_query, SLOT(updateQuery()));

		// Using updateQuery and not reload in the following connections since they use special queries and not directly displaying a database table.
		connect(photo_import, SIGNAL(modelChanged()), people_with_photos, SLOT(updateQuery()));
		connect(photo_import, SIGNAL(modelChanged()), people_with_photos_query, SLOT(updateQuery()));

		connect(photo_import, SIGNAL(modelChanged()), person_photos, SLOT(reload()));
	}

	void Document::filesystemWatchers() {
		album_template_watcher.addPath(System::albumTemplatesPath());
		picture_template_watcher.addPath(System::pictureTemplatesPath());
	}

}
