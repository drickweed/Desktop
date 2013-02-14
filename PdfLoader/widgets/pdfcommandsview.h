#pragma once

#include <QListView>

/*
	View of pdf graphics commands
*/
class PdfCommandsView : public QListView {
	Q_OBJECT

	public:
		PdfCommandsView(QWidget *parent=0);

		int currentRow() const {
			if (currentIndex().isValid())
				return currentIndex().row();
			return -1;
		}

	protected:
		void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

	signals:
		void rowSelected(int row);
};


