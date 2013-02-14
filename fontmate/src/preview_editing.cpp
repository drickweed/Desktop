#include "mainwindow.h"

namespace FontMate {

	void MainWindow::setBold(bool enabled) {
		if (fontPreviews()->empty())
			return;

		FontPreviewStack *preview = fontPreviews()->focusedPreview();

		if (not preview)
			return;

		preview->editor()->setBoldFormat(enabled);
		fontPreviews()->updateAllPreviews(preview);
	}

	void MainWindow::setItalic(bool enabled) {
		if (fontPreviews()->empty())
			return;

		FontPreviewStack *preview = fontPreviews()->focusedPreview();

		if (not preview)
			return;

		preview->editor()->setItalicFormat(enabled);
		fontPreviews()->updateAllPreviews(preview);
	}

	void MainWindow::setUnderline(bool enabled) {
		if (fontPreviews()->empty())
			return;

		FontPreviewStack *preview = fontPreviews()->focusedPreview();

		if (not preview)
			return;

		preview->editor()->setUnderlineFormat(enabled);
		fontPreviews()->updateAllPreviews(preview);
	}

	void MainWindow::setFontSize(int size) {
		if (fontPreviews()->empty())
			return;

		FontPreviewStack *preview = fontPreviews()->focusedPreview();

		if (not preview)
			return;

		preview->editor()->setSize(size);
		fontPreviews()->updateAllPreviews(preview);
	}

	void MainWindow::setDefaultFontSize(int font_size) {
		fontPreviews()->setDefaultFontSize(font_size);
	}

}
