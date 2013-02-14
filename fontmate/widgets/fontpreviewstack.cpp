#include "fontpreviewstack.h"

/** QT headers **/
#include <QDebug>
#include <QResizeEvent>
#include <QScrollBar>

namespace FontMate {

	FontPreviewStack::FontPreviewStack(QWidget *parent):
		QWidget(parent),
		m_sync_content(true) {
		preview = new FontPreviewContainer(this);
		label = new Label(this);
		setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		repositionLabel();

		connect(preview->closeButton(), SIGNAL(clicked()), SLOT(requestPreviewClose()));
		connect(editor(), SIGNAL(textChanged()), SLOT(editorContentsChangedEvent()));
		connect(editor(), SIGNAL(textChanged()), SLOT(updateLayout()));
		connect(preview->title_bar, SIGNAL(syncStateChanged(bool)), SLOT(setSyncContent(bool)));
	}

	FontPreviewStack::~FontPreviewStack() {
	}

	void FontPreviewStack::resizeEvent(QResizeEvent *event) {
		QWidget::resizeEvent(event);
		preview->resize(event->size());
		repositionLabel();
	}

	QSize FontPreviewStack::sizeHint() const {
		return QSize(200, 60);
	}

	void FontPreviewStack::repositionLabel() {
		int right_align = 1;
		if (editor()->verticalScrollBar()->isVisible())
			right_align = 17;


		label->resize(label->sizeHint());
		QPoint label_pos = QPoint(editor()->width(), editor()->height());
		label_pos -= QPoint(label->width() + right_align, label->height() + 1);
		label->move(label_pos);
	}

	void FontPreviewStack::setFont(QFont font) {
		setFont(font.family());
	}

	void FontPreviewStack::setFont(QString font) {
		label->setText(font);
		editor()->setFont(font);
		repositionLabel();
	}

}

void FontMate::FontPreviewStack::requestPreviewClose() {
	emit previewCloseRequested(this);
}

void FontMate::FontPreviewStack::editorContentsChangedEvent() {
	if (editor()->hasFocus())
		emit previewContentChanged(this);
}

void FontMate::FontPreviewStack::paintEvent(QPaintEvent * event) {
	QWidget::paintEvent(event);
	repositionLabel();
}

void FontMate::FontPreviewStack::updateLayout(){
	repositionLabel();
}
