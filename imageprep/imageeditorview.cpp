#include "imageeditorview.h"

ImageEditorView::ImageEditorView(QWidget *parent):
	QGraphicsView(parent) {
}

void ImageEditorView::resizeEvent(QResizeEvent *event) {
	QGraphicsView::resizeEvent(event);

}
