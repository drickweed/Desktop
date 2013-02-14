#include "pdfcommandsview.h"

PdfCommandsView::PdfCommandsView(QWidget *parent):
		QListView(parent) {
}

void PdfCommandsView::selectionChanged(const QItemSelection &selected, const QItemSelection & /* deselected */) {
	QModelIndexList indexes = selected.indexes();
	if (indexes.isEmpty())
		return;

	emit rowSelected(indexes[0].row());
}
