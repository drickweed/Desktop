#include "mainwindow.h"

namespace FontMate {
	void MainWindow::updateClearSelectionButton() {
		clearFontSelectionButton->setHidden(fonts()->selectionEmpty());
	}

	void MainWindow::clearFontSelection() {

		fontPreviews()->removeTemporaryView();

		// Start unselecting fonts from the preview.
		while (not fontPreviews()->empty())
			fonts()->unselectFont(0);

		// Unmark remaining fonts which were not in the previews
		fonts()->unmarkAll();

		updateClearSelectionButton();

		document->font_search_proxy->showOnlySelected(false);
		showSelectedFontsCheckbox->setChecked(false);
	}

	void MainWindow::updateSelectedFontsLabel() {
		showSelectedFontsCheckbox->setVisible(not fonts()->selectionEmpty() or document->font_search_proxy->showingSelected());

		int font_count = fonts()->selectedFontCount();
				selectionLabel->setText(QString("Selection (%1)").arg(font_count));
	}
}
