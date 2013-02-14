#include "fonteditorpreview.h"

/** QT headers **/
#include <QTextDocument>
#include <QTextFormat>
#include <QTextCursor>
#include <QApplication>
#include <QDebug>
#include <QMenu>

/** Application headers **/
#include <settings/settings.h>

namespace FontMate {

	FontEditorPreview::FontEditorPreview(QWidget* parent):
		QTextEdit(parent),
		zoom_mode(false) {
		connect(this, SIGNAL(selectionChanged()), SLOT(getSelectionFormat()));
	}

	FontEditorPreview::~FontEditorPreview() {
	}

	void FontEditorPreview::zoomIn() {
		resizeAllText(firstCharacterSize() + 1);
	}

	void FontEditorPreview::zoomOut() {
		if (firstCharacterSize() > 0)
			resizeAllText(firstCharacterSize() - 1);
	}

	int FontEditorPreview::firstCharacterSize() {

		moveCursor(QTextCursor::Start);
		QTextCursor cursor = textCursor();
		QTextCharFormat format = cursor.charFormat();
		return (int)format.fontPointSize();
	}

	void FontEditorPreview::keyPressEvent(QKeyEvent *event) {
		QTextEdit::keyPressEvent(event);
		if (event->key() == Qt::Key_Control) {
			zoom_mode = true;
		}
	}

	void FontEditorPreview::keyReleaseEvent(QKeyEvent *event) {
		QTextEdit::keyReleaseEvent(event);
		if (event->key() == Qt::Key_Control) {
			zoom_mode = false;
		}
	}

	void FontEditorPreview::wheelEvent ( QWheelEvent * event )  {
		QTextEdit::wheelEvent(event);
		//qDebug() << event->delta();
		if (zoom_mode) {
			if (event->delta() > 0)
				zoomIn();
			else if (event->delta() < 0)
				zoomOut();
		}
	}

	void FontEditorPreview::setFont(QString font_family) {
		m_font_family = font_family;
		updatePreviewFont();
	}

	void FontEditorPreview::setFont(QFont font) {
		setFont(font.family());
	}

	void FontEditorPreview::updatePreviewFont() {
		document()->setDefaultFont(QFont(fontFamily(), Settings().defaultSize()));

		QTextCursor cursor = textCursor();
		cursor.select(QTextCursor::Document);
		QTextCharFormat format;
		format.setFontFamily(fontFamily());
		cursor.mergeCharFormat(format);
	}

	void FontEditorPreview::copyTextFrom(FontEditorPreview *other) {
		QTextDocument *document = other->document();
		this->document()->setHtml(document->toHtml());
		updatePreviewFont();
	}

	void FontEditorPreview::setText(QString text) {
		clear();
		insertPlainText(text);
		updatePreviewFont();
	}

	void FontEditorPreview::setBoldFormat(bool enabled) {
		QTextCharFormat format;

		if (enabled)
			format.setFontWeight(QFont::Bold);
		else
			format.setFontWeight(QFont::Normal);

		setCursorFormat(format);
	}

	void FontEditorPreview::setUnderlineFormat(bool enabled) {

		QTextCharFormat format;

		if (enabled)
			format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
		else
			format.setUnderlineStyle(QTextCharFormat::NoUnderline);

		setCursorFormat(format);
	}

	void FontEditorPreview::setItalicFormat(bool enabled) {
		QTextCharFormat format;

		if (enabled)
			format.setFontItalic(enabled);
		else
			format.setFontItalic(enabled);

		setCursorFormat(format);
	}

	void FontEditorPreview::pasteHTML() {

	}

	void FontEditorPreview::contextMenuEvent(QContextMenuEvent *event) {
		QMenu *menu = createStandardContextMenu();
		menu->exec(event->globalPos());
		delete menu;
	}

	void FontEditorPreview::insertFromMimeData(const QMimeData *source) {
		QTextEdit::insertFromMimeData(source);
		setHtml(toHtml()); // Hack to cleanup pasted html. Otherwise cannoy apply formatting on inserted html when editing.
		updatePreviewFont();
	}
}

void FontMate::FontEditorPreview::getSelectionFormat() {
	QTextCursor cursor = textCursor();
	QTextCharFormat format = cursor.charFormat();

	emit selectionBold(format.fontWeight() == QFont::Bold);
	emit selectionUnderlined(format.underlineStyle() != QTextCharFormat::NoUnderline);
	emit selectionItalic(format.fontItalic());
	emit selectionSize(format.fontPointSize());
	emit selectionSize(QVariant(format.fontPointSize()).toString());
}

QTextCharFormat FontMate::FontEditorPreview::cursorFormat() const {
	QTextCursor cursor = textCursor();
	QTextCharFormat format;

	if (not cursor.hasSelection())
		format = cursor.blockCharFormat();
	else
		format = cursor.charFormat();

	return format;
}

void FontMate::FontEditorPreview::setCursorFormat(QTextCharFormat format) {
	mergeCurrentCharFormat(format);
	emit formattingChanged();
}

void FontMate::FontEditorPreview::setSize(int size) {
	QTextCharFormat format;
	format.setFontPointSize(size);
	setCursorFormat(format);
}

void FontMate::FontEditorPreview::resizeAllText(int size) {
	QTextCursor cursor = textCursor();
	cursor.select(QTextCursor::Document);
	QTextCharFormat format;
	format.setFontPointSize(size);
	cursor.mergeCharFormat(format);
}
