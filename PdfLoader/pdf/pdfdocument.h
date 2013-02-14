#pragma once

/** QT headers **/
#include <QList>
#include <QRectF>
#include <QImage>
#include <QObject>
#include <QRectF>
#include <QTransform>

/** Poppler qt4 headers **/
#include <poppler/qt4/poppler-qt4.h>

using namespace Poppler;

/*
	Convenience wrapper around poppler-qt4 pdf object
*/
class PdfDocument : public QObject {
	Q_OBJECT

	Q_PROPERTY(QRectF page_dimentions READ pageDim)
	Q_PROPERTY(QRectF last_image_rect READ lastImageRect)
	Q_PROPERTY(QRectF pdf_rect READ transformedPage)
	Q_PROPERTY(QString transform READ transformDisplay)
	Q_PROPERTY(int line_count READ lineCount)

	public:
		PdfDocument(QObject *parent=0);
		~PdfDocument();

		void load(QByteArray data);

		QByteArray data() const { return m_data; }

		int pageCount() const { return document->numPages(); }
		QImage renderPage(int index);

		bool isValid() const { return m_valid; }
		void setValid(bool valid=true) { m_valid = valid; }

		int lineCount() const { return m_lines.count(); }

		void setPageDim(QRectF page) { page_dimentions = page; }
		QRectF pageDim() const { return page_dimentions; }

		QRectF lastImageRect() const { return last_image_rect; }

		QRectF transformedPage() const { return m_last_transform.mapRect(pageDim()); }
		void setTransform(QTransform t) { m_last_transform = t; }
		QTransform transform() const { return m_last_transform; }
		QString transformDisplay() const { return QString("[%1, %2], [%3, %4], [%5, %6]")
										   .arg(m_last_transform.m11())
										   .arg(m_last_transform.m12())
										   .arg(m_last_transform.m21())
										   .arg(m_last_transform.m22())
										   .arg(m_last_transform.dx())
										   .arg(m_last_transform.dy()); }

		void addLine(QPointF line_position) { m_lines.append(line_position); }
		void initParsedInfo();
		QRectF lineBoundingRect(int line_number) const;
		bool validLineNumber(int line_number) const { return (line_number > 0 && line_number <= lineCount()); }

	private:
		Document *document;
		QList<QPointF> m_lines;
		QByteArray m_data;
		QRectF page_dimentions; // Current physical page dimentions
		QRectF last_image_rect; // Dimentions of last rendered image
		QTransform m_last_transform; // Last applied transformation

		bool m_valid;

		bool documentLoaded() const { return document != NULL; }

};
