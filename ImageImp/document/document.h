#pragma once

/** QT headers **/
#include <QObject>
#include <QFileSystemWatcher>

/** JB headers **/
#include <jb/qt4/search_proxy_model.h>
#include <jb/database/stored_order.h>
#include <jb/database/relation_proxy.h>

/** Application headers **/
/* Models */
#include <model/groups.h>
#include <model/people.h>
#include <model/albums.h>
#include <model/photodates.h>
#include <model/photo_import.h>
#include <model/photo_selection.h>
#include <model/people_with_photos.h>
#include <model/people_with_photos_query.h>
#include <model/templates.h>

/* Model proxies */
#include <model_proxies/unphotographed_people.h>

using namespace JB;

namespace ImageImp {

	/**
		ImageImp Document.

		Contains all the models referencing into the database.

		All model properties are public.
	*/
	class Document: public QObject {
		Q_OBJECT

		public:
			Document(QObject *parent=0);
			~Document() {}

			/**
				Setup the document models
			*/
			void setupModels();

			/**
				Setup the proxy models

				NOTE CAN ONLY be called after setting up the models.
			*/
			void setupProxyModels();

			/**
				Make connections between base models and the proxy models.

				Most of these connection are for invalidating proxy models when the base models change.
			*/
			void connectModelsToProxyModels();

			/** Models

				These models reference the main tables in the database. Groups of people, People in each group, and albums of each group. The models return different collection of objects based on the filters applied to them. They are also used in various proxy models in different areas in the application. See documentation for the meanings of phrases.
			**/
			Groups *groups; ///> Groups of people.
			People *people; ///> People in a single group.
			Albums *albums; ///> Albums for a group.

			PhotoImportModel *photo_import; ///> Photos to assign or already assigned to people.
			PhotoSelectionModel *person_photos; ///> Photos of a single person.

			PeopleWithPhotos *people_with_photos; ///> People who have images assigned to them.
			PeopleWithPhotosQuery *people_with_photos_query;
			PhotographedPeopleModel *photographed_people; ///> People who have been photographed.

			/** Photoshop templates **/
			TemplatesModel *picture_templates, ///> Picture templates.
						   *album_templates; ///> Album templates.

			/** Proxy models

				The rest of the models are proxy models for the above models.
			**/
			UnphotographedPeopleProxyModel *unphotographed_people; ///> People who havn't yet been photographed.
			RelationalProxyModel *photographed_people_names;

			/** The following are search proxies for above models with the corresponding names **/
			JB::SearchProxyModel *groups_search_proxy,
								 *people_search_proxy,
								 *albums_search_proxy,
								 *photo_import_people_search_proxy,
								 *photo_selection_people_search_proxy;

			/** Proxy for setting the order of people in an album **/
			StoredOrderProxyModel *ordered_people_proxy;

			RelationalObjectRef album() const { return m_album; }

		public slots:
			/**
				Reload all models in document.
			*/
			void reload();

			void setAlbum(RelationalObjectRef album);

			void setGroup(RelationalObjectRef group);

		private:
			RelationalObjectRef m_album; ///> Selected album.

			/**
				Create connections between models.
			*/
			void connectModels();

			void filesystemWatchers();

			QFileSystemWatcher album_template_watcher, picture_template_watcher;
	};

}
