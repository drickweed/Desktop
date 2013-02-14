#include "pdfscene.h"

#include <QGraphicsPixmapItem>

PdfScene::PdfScene(QObject *parent):
	QGraphicsScene(parent),
	m_page(NULL) {
	m_page = new QGraphicsPixmapItem;
	addItem(m_page);
}

void PdfScene::setImage(QImage image) {
	m_page->setPixmap(QPixmap::fromImage(image));
}
