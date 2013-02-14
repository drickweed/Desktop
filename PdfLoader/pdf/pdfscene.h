#pragma once

/** QT headers **/
#include <QGraphicsScene>

/*
	Pdf page viewer graphics scene.
*/
class PdfScene : public QGraphicsScene {
	Q_OBJECT

	public:
		PdfScene(QObject *parent=0);

		void setImage(QImage image);

	private:

		QGraphicsPixmapItem *m_page;
};
