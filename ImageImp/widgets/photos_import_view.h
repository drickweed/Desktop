#pragma once

#include <QListView>

namespace ImageImp {

	class PhotosImportView : public QListView {

		Q_OBJECT

		public:
			PhotosImportView(QWidget *parent);

		private slots:
			void contextMenu(const QPoint &point);
			void unassignSelectedPicture();

		private:
			QModelIndex m_selected_index;
	};

}
