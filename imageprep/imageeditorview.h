#pragma once

#include <QGraphicsView>

class ImageEditorView : public QGraphicsView {
	Q_OBJECT

	public:
		ImageEditorView(QWidget *parent=0);

	protected:
		void resizeEvent(QResizeEvent *event);
};

