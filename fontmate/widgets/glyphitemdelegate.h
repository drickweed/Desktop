#pragma once

/** QT headers **/
#include <QItemDelegate>

/** Application headers **/
#include "models/glyph.h"

namespace FontMate {

	class GlyphItemDelegate : public QItemDelegate {
		Q_OBJECT
		public:
			GlyphItemDelegate(QWidget *parent=0);

			void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

		signals:

		public slots:

	};
}
