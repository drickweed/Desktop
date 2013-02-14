#pragma once

#include <QListView>

namespace ImageImp {

	class PhotosSelectionView : public QListView {

		Q_OBJECT

		public:
			PhotosSelectionView(QWidget *parent);
			~PhotosSelectionView();

		private slots:
			void contextMenu(const QPoint &point);
// 			void unassignSelectedPicture();

		private:
			QModelIndex m_selected_index;
	};

}
