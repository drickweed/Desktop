#pragma once

/** QT headers **/
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QRect>
#include <QPointF>
#include <QList>
#include <QBrush>

/** Application headers **/
#include "translation.h"

class ImageEditorScene : public QGraphicsScene {
	Q_OBJECT

	public:
		ImageEditorScene(QObject *parent=0);
		virtual ~ImageEditorScene();

		void setImage(QString path);
		void setBackgroundImage(QString path);
		void clearBackgroundImage();
		void setCropSize(QSize size);
		void setCropPosition(QPoint point);
		void setScale(QPoint scale);
		void setRotate(qreal degrees);

		void setup();

		QBrush brush() const { return m_brush; }

		Translation currentTranslation() const;

		QRectF scaledImageRect() const;
		QSizeF scaledImageSize() const;

		QString imagePath() const { return m_image_path; }
		QString backgroundPath() const { return m_background_path; }

	public slots:
		void setCropHeight(int height);
		void setCropWidth(int width);
		void setCropTop(int top);
		void setCropLeft(int left);
		void setWidthScale(int width);
		void setHeightScale(int height);

	private slots:
		void updateSceneSize();
		/**
			Draw squares for the crop view
		*/
		void updateBorderRects();
		void updateImageScale();
		void updateCropRect();
		void updateCropView();
		void updateCropSettingsFromScene();
		void hideEditingControls();
		void showEditingControls();

	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

	private:

		QRectF cropRect() const { return QRectF(m_crop_position, m_crop_size); }
		QRectF imageRect() const { return m_pixmap->boundingRect(); }

		bool isImageLoaded() const { return not (m_pixmap == NULL); }
		bool backgroundLoaded() const { return not (m_background_image == NULL); }

		int width() const;
		int height() const;

		QGraphicsPixmapItem *m_pixmap,
							*m_background_image;
		QGraphicsRectItem *m_crop_rect;
		QList<QGraphicsRectItem*> m_border_rects;

		QBrush m_brush;
		QPen m_pen;
		QSize m_crop_size;
		QPoint m_crop_position;
		QPointF m_scale;
		QString m_image_path, m_background_path;
		bool m_scale_preserves_aspect_ratio;

	signals:
		void imageScaleChanged(const QRectF &scaled_rect);
		void cropRectMoved(const QRectF &crop_rect_current_position);
		void newImageLoaded();
};
