#pragma once

/** QT headers **/
#include <QListView>

#include <model/templates.h>

namespace ImageImp {

	/**
		Templates view.
	*/
	class TemplatesView : public QListView {
		Q_OBJECT

		public:
			TemplatesView(QWidget *parent);

			/**
				Path to selected template.

				@return Full path to selected template. Empty string if none selected.
			*/
			const QString selectedTemplatePath() const;

			const QString selectedTemplateName() const;

			/**
				Select template by name. If template not found or name is empty, first one is selected.

				@param template_name template to select.
			*/
			void selectTemplate(QString template_name);

			void saveSelection() const;

			void saveSelectionInSettings() const;

			/** QT api **/
			virtual void setModel(QAbstractItemModel *model);

		public slots:
			/**
				Load the template selection for the given album. If no saved selection available, select the first item in the model.
			*/
			void selectForAlbum(int album_id);

		private slots:
			/**
				Item context menu.
			*/
			void contextMenu(const QPoint &point);

			/**
				Open template in photoshop for editing.
			*/
			void editTemplate();

		protected slots:
			/** QT api **/
			void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

		private:
			QModelIndex m_selected_index; ///> Temporary storage for right click menu on items.

			/**
				Model cast to TemplatesModel.
			*/
			inline TemplatesModel *templatesModel() const;

			QModelIndex selectedTemplate() const;
	};

}
