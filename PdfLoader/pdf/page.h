#pragma once

/** QT headers **/
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QRectF>

/*
	Represents a PDF page.

	Contains:
	- Dimentions (Device space)
	- Content as a byte stream
 */
class Page : public QObject {
	Q_OBJECT

	Q_PROPERTY(QRectF dimentions READ dimentions WRITE setDimentions)

	public:
		Page(QObject *parent=0);

	QRectF dimentions() const { return m_dimentions; }
	void setDimentions(QRectF dimentions) { m_dimentions = dimentions; }

	private:
		qint32 m_object_id; /// Object ID reference in the original PDF
		QRectF m_dimentions; /// Page dimentions
		QByteArray m_content; /// Page content
};

