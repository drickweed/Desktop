#include "mainwindow.h"

/** QT headers **/
#include <QDebug>
#include <QFileDialog>
#include <QCompleter>
#include <QDirModel>
#include <QStatusBar>
#include <QMessageBox>

/** JB headers **/
#include <jb/std/memory.h>
#include <jb/qt4/widget.h>

/** Application headers **/
#include <settings/system.h>

/** STL headers **/
#include <stdlib.h>

namespace ImageImp {

	MainWindow::MainWindow(Document *document, QWidget* parent):
		QMainWindow(parent),
		document(document) {
		Ui_ImageImp::setupUi(this);

		setupSearchEditors();
		setupTableViews();

		setupUi();

		connectSettingsUi();
		connectViewSwitchingActions();
		photoImportingActions();
		connectUi();
		connectShowingRealWidgets();

		showItemEditor();

		selectFirstGroup();
	}

	void MainWindow::photoImportingActions() {
		// Populate photo model with pictures when the path changes.
		connect(photoImportPathEdit, SIGNAL(textChanged(const QString &)), document->photo_import, SLOT(populate(const QString &)));

		// Show path selection for photo import.
		connect(photoImportPathBrowseButton, SIGNAL(clicked()), SLOT(showPhotoImportPathDialog()));

		// Update zoom scale of imported pictures.
		connect(photoImportZoomSlider, SIGNAL(valueChanged(int)), document->photo_import, SLOT(setScale(int)));

		// Select the person to which to assign photos to when they are double clicked.
		connect(photoImportPeopleTableView, SIGNAL(objectSelected(RelationalObjectRef)), document->photo_import, SLOT(setPerson(RelationalObjectRef)));

		// Assign person selected in person view table to photo when doulbe clicked on.
		connect(photosImportView, SIGNAL(doubleClicked(const QModelIndex&)), document->photo_import, SLOT(assignPerson(const QModelIndex&)));

		// Update search proxy when searching for people to select images for.
		connect(photo_import_people_search_edit, SIGNAL(textEdited(const QString&)), document->photo_import_people_search_proxy, SLOT(setFilterFixedString(const QString&)));

		connect(autoAssighPhotos, SIGNAL(clicked()), SLOT(autoAssignPicturesToCurrentAlbum()));
		// Person <-> photo assignment changed. Invalidate the proxy model which filters out the people who has photographcs assigned to them
// 		connect(document->photo_import, SIGNAL(albumPhotoAssignmentChanged(int)), document->people_with_photos, SLOT(invalidate()));

		connect(loadExistingPhotosButton, SIGNAL(clicked()), document->photo_import, SLOT(populateFromDatabase()));

	}

	void MainWindow::connectUi() {

		/** Adding items to the database **/
		// Add item buttons (groups, albums and people)
		connect(gruopAddButton, SIGNAL(clicked()), document->groups, SLOT(appendNew()));
		connect(personAddButton, SIGNAL(clicked()), document->people, SLOT(appendNew()));
		connect(albumAddButton, SIGNAL(clicked()), document->albums, SLOT(appendNew()));

		/** Item removing **/
		connect(albumRemoveButton, SIGNAL(clicked()), SLOT(removeSelectedAlbum()));
		connect(personRemoveButton, SIGNAL(clicked()), SLOT(removeSelectedPerson()));
		connect(groupRemoveButton, SIGNAL(clicked()), SLOT(removeSelectedGroup()));

		/** Person ordering **/
		connect(moveTopButton, SIGNAL(clicked()), SLOT(orderedPeopleMoveTop()));
		connect(moveUpButton, SIGNAL(clicked()), SLOT(orderedPeopleMoveUp()));
		connect(moveDownButton, SIGNAL(clicked()), SLOT(orderedPeopleMoveDown()));
		connect(moveBottomButton, SIGNAL(clicked()), SLOT(orderedPeopleMoveBottom()));

		/** Model selection **/

		// Sync group table view and group combo box
		connect(groupsTableView, SIGNAL(indexSelected(int)), groupsComboBox, SLOT(setCurrentIndex(int)));
		connect(groupsComboBox, SIGNAL(currentIndexChanged(int)), groupsTableView, SLOT(selectRow(int)));

		// Sync album table view and album combo box
		connect(albumsTableView, SIGNAL(indexSelected(int)), albumsComboBox, SLOT(setCurrentIndex(int)));
		connect(albumsComboBox, SIGNAL(currentIndexChanged(int)), albumsTableView, SLOT(selectRow(int)));

		/** Managing ordered people in album **/
		connect(addPersonToAlbumButton, SIGNAL(clicked()), SLOT(addSelectedPersonToAlbum()));
		connect(addPersonToAlbumButton, SIGNAL(clicked()), document->unphotographed_people, SLOT(invalidate()));

		connect(removePersonFromAlbumButton, SIGNAL(clicked()), photographedPeopleTableView, SLOT(removeSelection()));
		connect(removePersonFromAlbumButton, SIGNAL(clicked()), document->unphotographed_people, SLOT(invalidate()));

		/** Setup column headers in views **/
		// Reset headers in the view when the model is reset due to filter changes.
		connect(document->people, SIGNAL(modelReset()), peopleTableView, SLOT(setupHeaders()));
		connect(document->people, SIGNAL(modelReset()), photoImportPeopleTableView, SLOT(setupHeaders()));
		connect(document->albums, SIGNAL(modelReset()), albumsTableView, SLOT(setupHeaders()));
		connect(document->photographed_people, SIGNAL(modelReset()), photographedPeopleTableView, SLOT(setupHeaders()));
		connect(document->people_with_photos, SIGNAL(modelReset()), peopleWithPhotosTableView, SLOT(setupHeaders()));
		connect(document->people_with_photos, SIGNAL(modelReset()), orderedPeopleTableView, SLOT(setupHeaders()));

		/** Selecting default photo for person in album **/
		connect(peopleWithPhotosTableView, SIGNAL(objectSelected(RelationalObjectRef)), document->person_photos, SLOT(setPerson(RelationalObjectRef)));
		connect(defaultPhotoSelectionZoomSlider, SIGNAL(valueChanged(int)), document->person_photos, SLOT(setScale(int)));
		connect(photosSelectionView, SIGNAL(doubleClicked(const QModelIndex&)), document->person_photos, SLOT(setDefaultPicture(const QModelIndex&)));

		connect(action_create_album, SIGNAL(triggered()), SLOT(createAlbum()));

		/** Photo import loading progress **/
		connect(document->photo_import, SIGNAL(imageProcessed(int)), progressBar, SLOT(step()));

		connect(document->photo_import, SIGNAL(processingStarted(int)), progressBar, SLOT(reset()));
		connect(document->photo_import, SIGNAL(processingStarted(int)), progressBar, SLOT(show()));
		connect(document->photo_import, SIGNAL(processingStarted(int)), progressBar, SLOT(setMaximum(int)));
		connect(document->photo_import, SIGNAL(processingStarted(int)), SLOT(disablImageAutoAssignButton()));

		connect(document->photo_import, SIGNAL(processingComplete()), progressBar, SLOT(hide()));
		connect(document->photo_import, SIGNAL(processingComplete()), SLOT(updateTimeDifference()));
		connect(document->photo_import, SIGNAL(processingComplete()), SLOT(enableImageAutoAssignButton()));

		/** Photo selection loading progress **/
		connect(document->person_photos, SIGNAL(imageProcessed(int)), progressBar, SLOT(step()));

		connect(document->person_photos, SIGNAL(processingStarted(int)), progressBar, SLOT(reset()));
		connect(document->person_photos, SIGNAL(processingStarted(int)), progressBar, SLOT(show()));
		connect(document->person_photos, SIGNAL(processingStarted(int)), progressBar, SLOT(setMaximum(int)));
		connect(document->person_photos, SIGNAL(processingStarted(int)), SLOT(disablImageAutoAssignButton()));

		connect(document->person_photos, SIGNAL(processingComplete()), progressBar, SLOT(hide()));
		connect(document->person_photos, SIGNAL(processingComplete()), SLOT(updateTimeDifference()));
		connect(document->person_photos, SIGNAL(processingComplete()), SLOT(enableImageAutoAssignButton()));

	}

	void MainWindow::setupUi() {

		/** Groups **/
		groupInstructions->setToolWidget(groupsTableView);
		if (not document->groups->empty())
			groupInstructions->showToolWidget();


		groupsComboBox->setModelColumn(Groups::NAME);
		groupsComboBox->setModel(document->groups);
		groupsComboBox->setModelColumn(Groups::NAME);

		/** Albums **/
		VBOX_LAYOUT(albumEditingControls)->addWidget(albumsTableView);

// 		albumInstructions->setToolWidget(albumsTableView);
// 		if (not document->albums->empty())
// 			albumInstructions->showToolWidget();

		albumsComboBox = new RelationalComboBox(this);
		albumsComboBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
		HBOX_LAYOUT(groupAlbumSelectors)->addWidget(albumsComboBox);
		setupAlbumSelectionUi();

		/** People **/
		QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(peopleEditingControls->layout());
		layout->addWidget(peopleTableView);
// 		peopleEditingButtons->addWidget(people_search_edit);

		/** Unordered people in an album**/
		QVBoxLayout *available_people_layout = VBOX_LAYOUT(availablePeopleControls);
		available_people_layout->addWidget(unphotographed_people_search_edit);
		available_people_layout->addWidget(unphotographedPeopleTableView);

		/** Ordered people in album **/
		QVBoxLayout *photographed_people_layout = qobject_cast<QVBoxLayout*>(albumOrderPeopleControls->layout());
		photographed_people_layout->addWidget(photographedPeopleTableView);

		/** Photos import view **/
		QCompleter *completer = new QCompleter(this);
		completer->setModel(new QDirModel(completer));
		photoImportPathEdit->setCompleter(completer);

		QVBoxLayout *photo_import_controls_layout = dynamic_cast<QVBoxLayout*>(photoImportPictureControls->layout());
		photo_import_controls_layout->insertWidget(1, photosImportView);

		photoImportPeopleLayout->insertWidget(1, photo_import_people_search_edit);
		photoImportPeopleLayout->insertWidget(2, photoImportPeopleTableView);
		photoImportPeopleTableView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

		/** Photo selection view **/
		QVBoxLayout *people_with_photos_layout = VBOX_LAYOUT(peopleWithPhotosSection);
		people_with_photos_layout->addWidget(peopleWithPhotosTableView);
		QVBoxLayout *select_photo_for_person_layout = qobject_cast<QVBoxLayout*>(pictureSelectionSection->layout());
		select_photo_for_person_layout->insertWidget(1,photosSelectionView);

		/** Ordered people **/
		orderedPeopleLayout->insertWidget(0, orderedPeopleTableView);

		templatesSelectionLayout->insertWidget(1, pictureTemplatesView);
		templatesSelectionLayout->insertWidget(3, albumTemplatesView);

		/** Progress bar **/
		progressBar = new ProgressBar(this);
		statusBar()->addPermanentWidget(progressBar);
		progressBar->hide();

		/** Manual time difference editor */
		manualTimeDifference->hide();
		earlierTimeDifference->hide();
	}

	void MainWindow::showPhotoImportPathDialog() {
		QString dir = QFileDialog::getExistingDirectory(this, tr("Select folder with photos"), System::defaultPicturesPath(), QFileDialog::ShowDirsOnly);

		if (not dir.isEmpty())
			photoImportPathEdit->setText(dir);
	}

	void MainWindow::updateGroupSelection(RelationalObjectRef group) {

		document->setGroup(group);

		// Model needs to be reset on combo box
		setupAlbumSelectionUi();

		if (group.isValid()) {
			enableButtons(peopleEditingControls);
			enableButtons(albumEditingControls);

		} else {
			disableButtons(albumEditingControls);
			disableButtons(peopleEditingControls);
		}

		document->ordered_people_proxy->invalidate();
	}

	void MainWindow::updateAlbumSelection(RelationalObjectRef album) {

		document->setAlbum(album);

		if (album.isValid()) {
			enableButtons(albumOrderPeopleControls);
			setChildrenEnabled(picturesPathSelectionControls, true);
		} else {
			disableButtons(albumOrderPeopleControls);
			setChildrenEnabled(picturesPathSelectionControls, false);
		}

		updateTimeDifference();
	}

	void MainWindow::setupAlbumSelectionUi() {
		albumsComboBox->setModel(document->albums, Albums::NAME);
	}

	int MainWindow::timeDifference() const {
		int hours = hourDifference->value();
		int minutes = minuteDifference->value();
		int seconds = secondDifference->value();
		bool negative = earlierTimeDifference->isChecked();

		int total_seconds = hours * 3600 + minutes * 60 + seconds;
		if (negative)
			total_seconds = -total_seconds;

		return total_seconds;
	}

	void MainWindow::enableButtons(QObject * parent) {
		JB::enableButtons(parent);
	}

	void MainWindow::disableButtons(QObject * parent) {
		JB::disableButtons(parent);
	}

	void MainWindow::enableImageAutoAssignButton() {
		autoAssighPhotos->setEnabled(true);
	}

	void MainWindow::disablImageAutoAssignButton() {
		autoAssighPhotos->setEnabled(false);
	}

	bool MainWindow::confirmRemoveDialog(QString type, QString name) {
		QStringList message_parts;
		message_parts << tr("Are you sure you want to delete") << name << "?";
		QMessageBox::StandardButton result = QMessageBox::question(this, tr("Delete ?"), message_parts.join(" "), QMessageBox::No | QMessageBox::Yes);
		return (result == QMessageBox::Yes);
	}

}

void ImageImp::MainWindow::updateTimeDifference() {
	int time_difference = document->photo_import->calculatedTimeDifference();

	earlierTimeDifference->setChecked(time_difference < 0);

	time_difference = abs(time_difference);

	hourDifference->setValue(time_difference < 3600 ? 0 : time_difference / 3600);
	time_difference = time_difference % 3600;

	minuteDifference->setValue(time_difference < 60 ? 0 : time_difference / 60);
	time_difference = time_difference % 60;

	secondDifference->setValue(time_difference < 60 ? time_difference : 0);
}

void ImageImp::MainWindow::connectShowingRealWidgets() {
	connect(gruopAddButton, SIGNAL(clicked()), groupInstructions, SLOT(showToolWidget()));
}
