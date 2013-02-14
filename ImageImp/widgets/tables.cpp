#include "mainwindow.h"

#include <QDebug>

/**
	This source file contains all code related to setting up the various table views in the application. Includes all code related to signal/slot connections both FROM and TO these tables.
*/

namespace ImageImp {

	void MainWindow::setupTableViews() {

		/** Groups **/
		groupsTableView = new GroupsView;
		groupsTableView->setModel(document->groups_search_proxy);
		groupsTableView->setupHeaders();

		// Upon selecting a group, the filters on other models and the availability of some
		// widgets changes.
		connect(groupsTableView, SIGNAL(objectSelected(RelationalObjectRef)), this, SLOT(updateGroupSelection(RelationalObjectRef)));

		/** Albums **/
		albumsTableView = new AlbumsView;
		albumsTableView->setModel(document->albums_search_proxy);
		albumsTableView->setupHeaders();

		connect(albumsTableView, SIGNAL(objectSelected(RelationalObjectRef)), this, SLOT(updateAlbumSelection(RelationalObjectRef)));

		/** People **/
		peopleTableView = new PeopleView(this);
		peopleTableView->setModel(document->people_search_proxy);
		peopleTableView->setupHeaders();

		/** Unphotographed people **/
		unphotographedPeopleTableView = new PeopleView(this);
		unphotographedPeopleTableView->setModel(document->unphotographed_people);

		/** Photographed people **/
		photographedPeopleTableView = new PhotographedPeopleView(this);
		photographedPeopleTableView->setModel(document->photographed_people_names);
		photographedPeopleTableView->setupHeaders();

		/** Photograph import photo view **/
		photosImportView = new PhotosImportView(this);
		photosImportView->setModel(document->photo_import);

		/** Photograph import people view **/
		photoImportPeopleTableView = new PeopleView(this);
		photoImportPeopleTableView->setModel(document->photo_import_people_search_proxy);
		photoImportPeopleTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		photoImportPeopleTableView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

		/** Photograph selection people view **/
		peopleWithPhotosTableView = new PeopleView(this);
		peopleWithPhotosTableView->setModel(document->people_with_photos);

		/** Photograph selection photo view **/
		photosSelectionView = new PhotosSelectionView(this);
		photosSelectionView->setModel(document->person_photos);

		/** People order view **/
		orderedPeopleTableView = new PeopleView(this);
		orderedPeopleTableView->setModel(document->ordered_people_proxy);
// 		orderedPeopleTableView->setSortingEnabled(true); // NOT USED: enables sorting by user.
		// The following 2 expressions force sorting. Sorting is turned on, but user cannot change sort order by clicking on the headers.
		orderedPeopleTableView->horizontalHeader()->setClickable(false);
		orderedPeopleTableView->sortByColumn(0, Qt::AscendingOrder);
// 		qDebug() << "Clickable" << orderedPeopleTableView->horizontalHeader()->isClickable();

		pictureTemplatesView = new TemplatesView(this);
		pictureTemplatesView->setModel(document->picture_templates);

		albumTemplatesView = new TemplatesView(this);
		albumTemplatesView->setModel(document->album_templates);

	}

}
