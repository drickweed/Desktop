#include "photos_import_view.h"

/** JB headers **/
#include "model/photo_import.h"

/** QT headers **/
#include <QMenu>

namespace ImageImp {

	PhotosImportView::PhotosImportView(QWidget * parent):
		QListView(parent) {
		setViewMode(QListView::IconMode);
		setSpacing(5);

		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenu(const QPoint&)));
	}

	void PhotosImportView::contextMenu(const QPoint &point) {
		QModelIndex index = indexAt(point);

		if (!index.isValid())
			return;

		QMenu menu;

		m_selected_index = index;

		QAction *action = menu.addAction(QString("Unassign person"));
		connect(action, SIGNAL(triggered()), SLOT(unassignSelectedPicture()));

		menu.exec(viewport()->mapToGlobal(point));
	}

	void PhotosImportView::unassignSelectedPicture() {
		PhotoImportModel *photos_model = dynamic_cast<PhotoImportModel*>(model());

		if (not photos_model)
			return;

		photos_model->unassignPerson(m_selected_index);
	}

}
