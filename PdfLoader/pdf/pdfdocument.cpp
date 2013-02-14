#include "pdfdocument.h"

/** QT headers **/
#include <QDebug>

/** Application headers **/
#include <graphics/parser.h>

using namespace Poppler;

PdfDocument::PdfDocument(QObject *parent) :
	QObject(parent),
	document(NULL),
	m_valid(false) {
}

PdfDocument::~PdfDocument() {

}

QRectF PdfDocument::lineBoundingRect(int line_number) const {
	QRectF rect;
	rect.setTopLeft(m_lines[line_number-1]);
	rect.setWidth(pageDim().width());
	rect.setHeight(50);
	rect.translate(0, -25);
	qDebug() << "Line rectangle" << rect;
	return rect;
}

void PdfDocument::load(QByteArray data) {
	if (documentLoaded())
		delete document;

	m_data = data;

	document = Document::loadFromData(data);
	setValid();

}

void PdfDocument::initParsedInfo() {
	m_lines.clear();
}

QImage PdfDocument::renderPage(int index) {
	if (not documentLoaded())
		return QImage();

	QImage image = document->page(index)->renderToImage();
	last_image_rect = image.rect();
	return image;
}
