#include "filechecksumtable.h"

/** QT headers **/
#include <QMenu>
#include <QAction>

namespace MiniSFV {

	FileChecksumTable::FileChecksumTable(QWidget *parent):
		QTableView(parent) {

		setSelectionBehavior(QAbstractItemView::SelectRows);

		setDragEnabled(true);
		setAcceptDrops(true);
		setDropIndicatorShown(true);

		verticalHeader()->hide();
		horizontalHeader()->setResizeMode(QHeaderView::Stretch);
		horizontalHeader()->hide();

		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(contextMenu(const QPoint&)));

		QObject::connect(this, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(itemActivated(const QModelIndex&)));
	}

	void FileChecksumTable::forwardedDropEvent(QDropEvent *event) {
		dropEvent(event);
	}

	void FileChecksumTable::scrollToArea(QModelIndex start, QModelIndex /* end */) {
		scrollTo(start);
	}

	void FileChecksumTable::contextMenu(const QPoint & point) {

		QModelIndex index = indexAt(point);
		FileChecksumModelProxy *proxy = dynamic_cast<FileChecksumModelProxy*>(model());
		index = proxy->mapToSource(index);

		if (not index.isValid())
			return;

		QMenu menu;
		QAction *action = menu.addAction(tr("Remove"));
		connect(action, SIGNAL(triggered()), SLOT(removeSelected()));

		m_selected_row = index.row();

		menu.exec(viewport()->mapToGlobal(point));
	}

	void FileChecksumTable::removeSelected() {
		sourceModel()->remove(m_selected_row);
	}

	void FileChecksumTable::itemActivated(const QModelIndex &index) {
		QModelIndex model_index = proxyModel()->mapToSource(index);
		emit itemSelected(model_index);
	}

	FileChecksumModel* FileChecksumTable::sourceModel() const {
		FileChecksumModelProxy *proxy = dynamic_cast<FileChecksumModelProxy*>(model());
		FileChecksumModel *model = dynamic_cast<FileChecksumModel*>(proxy->sourceModel());
		return model;
	}

	FileChecksumModelProxy* FileChecksumTable::proxyModel() const {
		FileChecksumModelProxy *proxy = dynamic_cast<FileChecksumModelProxy*>(model());
		return proxy;
	}

}


