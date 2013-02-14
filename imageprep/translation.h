#pragma once

/** QT headers **/
#include <QSize>
#include <QPoint>
#include <QPointF>
#include <QSizeF>
#include <QString>

class Translation {
	public:
		Translation();
		Translation(QPoint crop_position, QSize crop, QPointF scale);

		QSize crop; ///> Size to cut from original image
		QPoint crop_position; ///> Position to cut from original image
		QPointF scale; ///> Size after cutting from original image
		QString background_file_path; ///> Full path to background file

		bool valid() const;

		bool apply(QString full_source_path, QString destination_full_path) const;

		static bool deleteExisting;
};
