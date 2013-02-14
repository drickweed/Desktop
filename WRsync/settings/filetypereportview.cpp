#include "filetypereportview.h"

#include <QMenu>
#include <QAction>

FileTypeReportView::FileTypeReportView(QWidget *parent) :
	QTableView(parent) {
	setContextMenuPolicy (Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(contextMenu(const QPoint&)));
}


void FileTypeReportView::contextMenu(const QPoint & point) {

	QModelIndex index = indexAt(point);
	if (not index.isValid())
		return;

	// Find selected dates
	QItemSelectionModel *selection_model = selectionModel();
	QModelIndexList selection = selection_model->selectedIndexes();
	selected_extension = model()->data(selection[0]).toString();

	QMenu menu;
	QAction *action;
	action = menu.addAction(tr("Filter"));

	menu.exec();
}

void FileTypeReportView::createFilter() {
	emit filterRequest(selected_extension);
}
