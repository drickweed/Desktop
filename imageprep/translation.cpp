#include "translation.h"

/** QT headers **/
#include <QImage>
#include <QDebug>
#include <QFileInfo>
#include <QPainter>

/** JB headers **/
#include <jb/qt4/pathutils.h>

using namespace JB;

bool Translation::deleteExisting = true;

Translation::Translation() {
}

Translation::Translation(QPoint crop_position, QSize crop, QPointF scale):
		crop(crop),
		crop_position(crop_position),
		scale(scale) {

}

bool Translation::valid() const {
	return crop.isValid();
}


bool Translation::apply(QString full_source_path, QString destination_full_path) const {
	QSize real_crop;
	QImage image(full_source_path);

	if (image.isNull()) {
		qDebug() << "No image given for processing";
		return false;
	}

	// Scale image
	QImage dest_image = image.scaled((scale.x() / 100) * image.width(), (scale.y() / 100) * image.height(), Qt::KeepAspectRatio);

	// Minimize cropping to fit scaled image size
	real_crop.setWidth(qMin(dest_image.width(), crop.width()));
	real_crop.setHeight(qMin(dest_image.height(), crop.height()));

	// Crop image
	dest_image = dest_image.copy(QRect(crop_position, real_crop));

	if (not background_file_path.isEmpty()) {
		QImage background(background_file_path);
		if (not background.isNull()) {
			QRect centered_image = dest_image.rect();
			centered_image.moveCenter(background.rect().center());
			QPainter painter(&background);
			painter.drawImage(centered_image, dest_image);
			painter.end();
			dest_image = background;
		}
	}

	QFileInfo source_image_file(full_source_path);
	destination_full_path = Path::join(destination_full_path, source_image_file.fileName());

	if (QFileInfo(destination_full_path).exists()) {
		if (Translation::deleteExisting) {
			if (not QFile(destination_full_path).remove())
				return false;
		} else {
			return false;
		}
	}

	return dest_image.save(destination_full_path);
}
