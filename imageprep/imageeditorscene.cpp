#include "imageeditorscene.h"

/** QT headers **/
#include <QColor>
#include <QPixmap>
#include <QDebug>
#include <QTransform>

/** JB headers **/
#include <jb/std/memory.h>

ImageEditorScene::ImageEditorScene(QObject *parent):
	QGraphicsScene(parent),
	m_pixmap(NULL),
	m_background_image(NULL),
	m_crop_rect(NULL),
	m_crop_position(QPoint(0,0)),
	m_scale(QPoint(100, 100)),
	m_scale_preserves_aspect_ratio(true) {
	setup();
}

ImageEditorScene::~ImageEditorScene() {
	DELETE(m_pixmap);
	DELETE(m_background_image);
}

void ImageEditorScene::setup() {
	// Crop rectangle border color
	QColor color(Qt::black);

	color.setAlpha(99);
	m_brush.setColor(color);
	m_brush.setStyle(Qt::SolidPattern);

	m_pen = QPen(m_brush, 0);

	// Create crop rectangles
	QRectF blank_rect(0,0,0,0);
	for (int i=0; i<4; i++) {
		m_border_rects.append(addRect(blank_rect, m_pen, m_brush));
		m_border_rects[i]->setZValue(4);
	}

	m_crop_rect = addRect(blank_rect, QPen(), QBrush());
	m_crop_rect->setAcceptDrops(true);
	m_crop_rect->setFlag(QGraphicsItem::ItemIsMovable, true);
	m_crop_rect->setZValue(6);

	hideEditingControls();

	connect(this, SIGNAL(imageScaleChanged(const QRectF &)), SLOT(updateBorderRects()));
	connect(this, SIGNAL(imageScaleChanged(const QRectF &)), SLOT(updateImageScale()));
	connect(this, SIGNAL(imageScaleChanged(const QRectF &)), SLOT(updateSceneSize()));

	connect(this, SIGNAL(cropRectMoved(const QRectF&)), SLOT(updateBorderRects()));

	connect(this, SIGNAL(newImageLoaded()), SLOT(updateImageScale()));
	connect(this, SIGNAL(newImageLoaded()), SLOT(updateCropView()));
	connect(this, SIGNAL(newImageLoaded()), SLOT(updateSceneSize()));
	connect(this, SIGNAL(newImageLoaded()), SLOT(showEditingControls()));
}

void ImageEditorScene::hideEditingControls() {
	for (int i=0; i<4; i++)
		m_border_rects[i]->hide();
	m_crop_rect->hide();
}

void ImageEditorScene::showEditingControls() {
	for (int i=0; i<4; i++)
		m_border_rects[i]->show();
	m_crop_rect->show();
}

void ImageEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	QGraphicsScene::mouseMoveEvent(mouseEvent);

	if (mouseGrabberItem() == NULL) {
		return;
	}

	updateCropSettingsFromScene();
	emit cropRectMoved(m_crop_rect->mapToScene(m_crop_rect->boundingRect()).boundingRect());
}

QSizeF ImageEditorScene::scaledImageSize() const {
	QSizeF size;
	size.setWidth(m_pixmap->boundingRect().width() * (m_scale.x() / 100));
	size.setHeight(m_pixmap->boundingRect().height() * (m_scale.y() / 100));
	return size;
}

QRectF ImageEditorScene::scaledImageRect() const {
	QRectF rect;
	rect.setTopLeft(m_pixmap->pos());
	rect.setSize(scaledImageSize());
	return rect;
}

void ImageEditorScene::setImage(QString path) {
	QPixmap pixmap(path);
	if (m_pixmap) {
		removeItem(m_pixmap);
		DELETE(m_pixmap);
	}

	if (pixmap.isNull()) {
		hideEditingControls();
		return;
	}

	m_pixmap = addPixmap(pixmap);
	m_pixmap->setZValue(2);
	m_image_path = path;

	emit newImageLoaded();
	updateImageScale();
	updateCropView();
	updateSceneSize();
}

void ImageEditorScene::setBackgroundImage(QString path) {
	clearBackgroundImage();
	QPixmap pixmap(path);
	m_background_path = path;
	m_background_image = addPixmap(pixmap);
	m_background_image->setZValue(1);
}

void ImageEditorScene::clearBackgroundImage() {
	if (m_background_image) {
		removeItem(m_background_image);
		DELETE(m_background_image);
		m_background_path = "";
	}
}

Translation ImageEditorScene::currentTranslation() const {
	Translation translation;
	translation.crop = m_crop_size;
	translation.scale = m_scale;
	translation.crop_position = m_crop_position;
	if (backgroundLoaded())
		translation.background_file_path = backgroundPath();
	return translation;
}

void ImageEditorScene::updateSceneSize() {
	setSceneRect(m_pixmap->mapToScene(m_pixmap->boundingRect()).boundingRect());
}

void ImageEditorScene::updateBorderRects() {
	if (not isImageLoaded())
		return;

	// Top
	m_border_rects[0]->setRect(QRectF(scaledImageRect().topLeft(), QPointF(scaledImageRect().right(), m_crop_position.y())));

	// Left
	m_border_rects[1]->setRect(QRectF(QPointF(0, cropRect().top()+1), cropRect().bottomLeft() - QPointF(0, 1)));

	// Bottom
	m_border_rects[2]->setRect(QRectF(QPointF(0, cropRect().bottom()), scaledImageRect().bottomRight()));

	// Right
	m_border_rects[3]->setRect(QRectF(QPointF(cropRect().right(), cropRect().top()+1), QPointF(scaledImageRect().right(), cropRect().bottom())- QPointF(0, 1)));

}

void ImageEditorScene::updateCropRect() {

	m_crop_rect->setRect(m_crop_rect->mapFromScene(QRectF(m_crop_position, m_crop_size)).boundingRect());
}

void ImageEditorScene::setScale(QPoint scale) {
	m_scale = scale;
	updateImageScale();
}

void ImageEditorScene::setWidthScale(int width) {
	if ((width < 1) or (width > 100))
		return;

	m_scale.setX(width);

	if (m_scale_preserves_aspect_ratio)
		m_scale.setY(width);

	if (isImageLoaded())
		emit imageScaleChanged(scaledImageRect());
}

void ImageEditorScene::updateCropView() {
	updateCropRect();
	updateBorderRects();
}

void ImageEditorScene::updateCropSettingsFromScene() {
	m_crop_size = QSize((int)m_crop_rect->boundingRect().width(), (int)m_crop_rect->boundingRect().height());
	m_crop_position = QVariant(m_crop_rect->scenePos()).toPoint();
}

void ImageEditorScene::setHeightScale(int height) {
	if ((height < 1) or (height > 100))
		return;

	m_scale.setY(height);

	if (m_scale_preserves_aspect_ratio)
		m_scale.setX(height);

	if (isImageLoaded())
		emit imageScaleChanged(scaledImageRect());
}

int ImageEditorScene::width() const {
	if (not m_pixmap)
		return 0;
	return m_pixmap->boundingRect().width();
}

int ImageEditorScene::height() const {
	if (not m_pixmap)
		return 0;
	return m_pixmap->boundingRect().height();
}

void ImageEditorScene::updateImageScale() {
	if (not isImageLoaded())
		return;

	m_pixmap->setTransform(QTransform::fromScale(m_scale.x() / 100, m_scale.x() / 100), false);
}

void ImageEditorScene::setCropHeight(int height) {
	m_crop_size.setHeight(height);
	updateCropView();
}

void ImageEditorScene::setCropWidth(int width) {
	m_crop_size.setWidth(width);
	updateCropView();
}

void ImageEditorScene::setCropTop(int top) {
	m_crop_position.setY(top);
	updateCropView();
}

void ImageEditorScene::setCropLeft(int left) {
	m_crop_position.setX(left);
	updateCropView();
}

void ImageEditorScene::setCropSize(QSize size) {
	m_crop_size = size;
	updateCropView();
}

void ImageEditorScene::setCropPosition(QPoint point) {
	m_crop_position = point;
	updateCropView();
}
