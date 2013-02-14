#include "widgets/photos_selection_view.h"

#include "model/photo_import.h"

#include <QMenu>

namespace ImageImp {

	PhotosSelectionView::PhotosSelectionView(QWidget * parent):
		QListView(parent) {
		setViewMode(QListView::IconMode);
		setSpacing(5);

		// Menu
		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenu(const QPoint&)));
	}

	PhotosSelectionView::~PhotosSelectionView() {
	}

}

void ImageImp::PhotosSelectionView::contextMenu(const QPoint &point) {
	QModelIndex index = indexAt(point);
	if (!index.isValid())
		return;

// 	QMenu *menu = new QMenu(this);

	m_selected_index = index;

// 	QAction *action = menu->addAction(QString("Unassign person"));
// 	connect(action, SIGNAL(triggered()), this, SLOT(unassignSelectedPicture()));

// 	menu->exec(viewport()->mapToGlobal(point));
}

// void ImageImp::PhotosSelectionView::unassignSelectedPicture() {
// 	PhotoImportModel *photos_model = dynamic_cast<PhotoImportModel*>(model());
//
// 	if (not photos_model)
// 		return;
//
// 	photos_model->unassignPerson(m_selected_index);
// }
