#pragma once

/** QT headers **/
#include <QItemDelegate>
#include <QFontMetrics>

/** Application headers **/
#include <models/fonts.h>
#include <models/fontfilter.h>

namespace FontMate {

	/**
		Renders a font in the font list.
	*/
	class FontItemDelegate : public QItemDelegate {
		public:
			FontItemDelegate(QObject *parent=0);

			~FontItemDelegate();

			void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
			Fonts *fonts_model;
			FontFilter *proxy_model;

		private:
			QFontMetrics *fm;

			void paintShowMoreItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
	};

}
