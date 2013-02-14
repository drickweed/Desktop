#include "mainwindow.h"

/**
	This source file contains all methods related to switching betweeen the various application views.
*/

namespace ImageImp {

	void MainWindow::connectViewSwitchingActions() {

		// Switch to item editor view. Adding and removing groups, albums and people.
		connect(action_show_item_editor, SIGNAL(triggered()), this, SLOT(showItemEditor()));

		// Switch to ui which allows selecting the order in which photographs are taken.
		connect(action_take_photos, SIGNAL(triggered()), this, SLOT(showPictureTakingEditor()));

		// Switch to ui which allows assigning photographs to people.
		connect(action_import_photos, SIGNAL(triggered()), this, SLOT(showPictureImporter()));

		// Switch to ui which allows selecting default photo for person.
		connect(action_select_default_photos, SIGNAL(triggered()), this, SLOT(showPictureSelector()));

		// Switch to ui which allows setting album import settings and person order in album.
		connect(action_show_album_settings_editor, SIGNAL(triggered()), this, SLOT(showAlbumSettingsEditor()));

	}

	void MainWindow::showItemEditor() {
		action_take_photos->setChecked(false);
		action_import_photos->setChecked(false);
		action_select_default_photos->setChecked(false);
		action_show_album_settings_editor->setChecked(false);

		groupAlbumSelectors->hide();
		topViewsStack->setCurrentIndex(Views::ItemEditor);
	}

	void MainWindow::showPictureTakingEditor() {
		action_show_item_editor->setChecked(false);
		action_import_photos->setChecked(false);
		action_select_default_photos->setChecked(false);
		action_show_album_settings_editor->setChecked(false);

		groupAlbumSelectors->show();
		topViewsStack->setCurrentIndex(Views::PersonOrder);
	}

	void MainWindow::showPictureImporter() {
		action_show_item_editor->setChecked(false);
		action_take_photos->setChecked(false);
		action_select_default_photos->setChecked(false);
		action_show_album_settings_editor->setChecked(false);

		groupAlbumSelectors->show();
		topViewsStack->setCurrentIndex(Views::PhotoAssign);
	}

	void MainWindow::showPictureSelector() {
		action_show_item_editor->setChecked(false);
		action_take_photos->setChecked(false);
		action_import_photos->setChecked(false);
		action_show_album_settings_editor->setChecked(false);

		groupAlbumSelectors->show();
		topViewsStack->setCurrentIndex(Views::PhotoSelect);
	}

	void MainWindow::showAlbumSettingsEditor() {
		action_show_item_editor->setChecked(false);
		action_take_photos->setChecked(false);
		action_import_photos->setChecked(false);
		action_select_default_photos->setChecked(false);

		groupAlbumSelectors->show();
		topViewsStack->setCurrentIndex(Views::TemplateEdit);
	}

}
