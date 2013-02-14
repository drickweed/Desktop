#pragma once

/** QT headers **/
#include <QMainWindow>

/** JB headers **/
#include <jb/qt4/widget.h>
#include <jb/database/relation_proxy.h>
#include <jb/qt4/progress.h>
#include <jb/database/combobox.h>

// User Interface
#include "ui_imageimp.h"

/** Application headers **/
#include "document/document.h"

#include "widgets/groups_view.h"
#include "widgets/albums_view.h"
#include "widgets/people_view.h"
#include "widgets/photodates_view.h"
#include "widgets/photos_import_view.h"
#include "widgets/photos_selection_view.h"
#include "widgets/templates_view.h"

using namespace JB;

namespace ImageImp {

	namespace Views {
		enum ViewNames {ItemEditor, PersonOrder, PhotoAssign, PhotoSelect, TemplateEdit};
	}

	/**
		Main ImageImp GUI.
	*/
	class MainWindow : public QMainWindow, private Ui::ImageImp {

		Q_OBJECT

		public:
			MainWindow(Document *document, QWidget *parent=0);

		private: /** Methods **/
			void connectUi();

			/**
				Connect actions which switch between UI views.
			*/
			void connectViewSwitchingActions();
			void photoImportingActions();

			/**
				Setup search line editors.
			*/
			void setupSearchEditors();

			void connectShowingRealWidgets();

			/**
				Setup table views
			*/
			void setupTableViews();

			void setupUi();

			/**
				Setup album selection ui
			*/
			void setupAlbumSelectionUi();

			/**
				Select the first group.
			*/
			void selectFirstGroup();

		private: /** Properties **/

			Document *document;

			/** Widgets **/
			GroupsView *groupsTableView;
			AlbumsView *albumsTableView;
			PeopleView *peopleTableView,
					   *unphotographedPeopleTableView,
					   *photoImportPeopleTableView,
					   *defaultPhotoSelectionPeopleTableView,
					   *peopleWithPhotosTableView,
					   *orderedPeopleTableView;

			RelationalComboBox *albumsComboBox;

			PhotographedPeopleView *photographedPeopleTableView;
			PhotosImportView *photosImportView;
			PhotosSelectionView *photosSelectionView;

			TemplatesView *pictureTemplatesView,
						  *albumTemplatesView;

			ProgressBar *progressBar;

			int timeDifference() const;

		public slots:
			/** Switch between views **/
			void showItemEditor();
			void showPictureTakingEditor();
			void showPictureImporter();
			void showPictureSelector();
			void showAlbumSettingsEditor();

			/** Enable buttons **/
			void enableButtons(QObject *parent);

			/** Disable views **/
			void disableButtons(QObject *parent);

			/** Adding order to people in an album **/
			void addSelectedPersonToAlbum();

			/** Photo import **/
			void showPhotoImportPathDialog();

			void autoAssignPicturesToCurrentAlbum();

			void createAlbum();

			/** Store settings **/
			void setWidth(int i);
			void setHeight(int i);
			void connectSettingsUi();

			/**
				Display removal confirmation dialog.

				@param type type of object being removed
				@param name name of object being removed
				@return confirmation result.
			*/
			bool confirmRemoveDialog(QString type, QString name);

		private slots:
			void updateGroupSelection(RelationalObjectRef group);
			void updateAlbumSelection(RelationalObjectRef album);

			void enableImageAutoAssignButton();
			void disablImageAutoAssignButton();

			void orderedPeopleMoveTop();
			void orderedPeopleMoveUp();
			void orderedPeopleMoveDown();
			void orderedPeopleMoveBottom();

			void removeSelectedAlbum();
			void removeSelectedPerson();
			void removeSelectedGroup();

			void updateTimeDifference();

			/**
				Calculate time different between camera and computer from selected person and image.
			*/
			void syncTimeFromSelection();
	};

}
