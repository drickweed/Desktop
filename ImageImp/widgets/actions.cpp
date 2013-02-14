#include "mainwindow.h"

/** QT headers **/
#include <QMessageBox>

/** JB headers **/
#include <jb/std/log.h>

/** Application headers **/
#include <processor/create_album.h>
#include <processor/clean_db.h>

namespace ImageImp {

	void MainWindow::addSelectedPersonToAlbum() {

		if (not document->photographed_people->albumSelected())
			return;

		QModelIndexList selection = unphotographedPeopleTableView->selectionModel()->selectedRows();

		if (selection.empty())
			return;

		QModelIndex index = document->unphotographed_people->mapToSource(selection[0]);

		Person *person = dynamic_cast<Person*>(document->people->getObject(index.row()));
		PhotoDate *photodate = new PhotoDate(person);
		document->photographed_people->append(photodate);
		if (photodate) { delete photodate; photodate = NULL;}
		if (person) { delete person; person = NULL;}
	}

	void MainWindow::selectFirstGroup() {
		if (document->groups->rowCount() > 0)
			groupsTableView->selectRow(0);
	}

	void MainWindow::autoAssignPicturesToCurrentAlbum() {
		int time_difference = 0;

		LOG("Auto assigning people to images");

		if (automaticTimeDifferenceButton->isChecked()) {
			LOG("Using automatic time difference");
			time_difference = document->photo_import->calculatedTimeDifference();
		} else {
			LOG("Using manual time difference");
			time_difference = timeDifference();
		}

		document->photo_import->autoAssign(time_difference);
	}

	void MainWindow::createAlbum(){
		AlbumCreator album_creator;
		try {
			album_creator.setAlbum(document->album());
			album_creator.createAlbum();
		} catch (UnsupportedPlatform *e) {
			QMessageBox::information(this, tr("Unsupported platform"), tr("Exporting to photoshop not supported on this platform"));
		} catch (AlbumIsEmpty *e) {
			QMessageBox::information(this, tr("Album is empty"), tr("No images found to make an album from"));
		} catch (NoAlbumSelected *e) {
			QMessageBox::information(this, tr("No album selected"), tr("No album selected"));
		} catch (std::exception *e) {
			// Ignore unknown errors.
		}
	}

	void MainWindow::orderedPeopleMoveTop() {
		QModelIndex selected = orderedPeopleTableView->selectedIndex();
		if (selected.isValid())
			document->ordered_people_proxy->moveTop(selected);
	}

	void MainWindow::orderedPeopleMoveUp() {
		QModelIndex selected = orderedPeopleTableView->selectedIndex();
		if (selected.isValid())
			document->ordered_people_proxy->moveUp(selected);
	}

	void MainWindow::orderedPeopleMoveDown() {
		QModelIndex selected = orderedPeopleTableView->selectedIndex();
		if (selected.isValid())
			document->ordered_people_proxy->moveDown(selected);
	}

	void MainWindow::orderedPeopleMoveBottom() {
		QModelIndex selected = orderedPeopleTableView->selectedIndex();
		if (selected.isValid())
			document->ordered_people_proxy->moveBottom(selected);
	}

	void MainWindow::removeSelectedAlbum() {
		QModelIndex index = albumsTableView->selectedIndex();
		if (not index.isValid())
			return;

		if (not confirmRemoveDialog(tr("album"), document->albums->record(index.row()).value("name").toString()))
			return;

		document->albums->remove(index);
	}

	void MainWindow::removeSelectedPerson() {
		QModelIndex index = peopleTableView->selectedIndex();
		if (not index.isValid())
			return;

		if (not confirmRemoveDialog(tr("person"), document->people->record(index.row()).value("name").toString()))
			return;

		document->people->remove(index);

		document->people_with_photos->updateQuery();
		document->people_with_photos_query->updateQuery();
	}

	void MainWindow::removeSelectedGroup() {
		QModelIndex index = groupsTableView->selectedIndex();
		if (not index.isValid())
			return;

		if (not confirmRemoveDialog(tr("group"), document->groups->record(index.row()).value("name").toString()))
			return;

		document->groups->remove(index);

		document->reload();
	}

	void MainWindow::syncTimeFromSelection() {

	}
}
