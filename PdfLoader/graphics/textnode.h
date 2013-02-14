#pragma once

/** QT headers **/
#include <QTransform>

/*
	Represents some of the info needed to render text on the current page:
	- Location
	- Text
	- Size
  */
class TextNode {
	public:
		TextNode();

		qreal size;
		QByteArray text;

		void applyMatrix(QTransform matrix) { m_matrix = matrix * m_matrix; }
		void applyDeltaMatrix(QPointF delta);
		QTransform matrix() const { return m_matrix;}

		QPointF position() const { return QPointF(matrix().dx(), matrix().dy()); }

	private:
		QTransform m_matrix;

};
