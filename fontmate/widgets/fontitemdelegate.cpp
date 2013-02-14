#include "fontitemdelegate.h"

/** QT headers **/
#include <QDebug>
#include <QApplication>
#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QColor>

const int font_family_name_margin = 3;

namespace FontMate {

	FontItemDelegate::FontItemDelegate(QObject *parent):
		QItemDelegate(parent),
		fonts_model(NULL),
		proxy_model(NULL) {
		fm = new QFontMetrics(qApp->font());
	}

	FontItemDelegate::~FontItemDelegate() {
		delete fm;
	}

	void FontItemDelegate::paintShowMoreItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & /* index */) const {

		QString show_more_items_message = QString("Showing %1 fonts. Show more...").arg(proxy_model->displayedFontCount());

		painter->drawText(option.rect.right() - fm->width(show_more_items_message), option.rect.top() + fm->height(), show_more_items_message);
	}

	void FontItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const {

		QItemDelegate::paint(painter, option, index);

		if (not fonts_model)
			return;

		if (proxy_model) {
			if (proxy_model->showMoreItem(index)) {
				paintShowMoreItem(painter, option, index);
				return;
			}
		}

		if (not fonts_model->inlinePreview())
			return;

		QString font_family = index.model()->data(index, Qt::UserRole).toString();

		QRect rect(option.rect.bottomRight() - QPoint(fm->width(font_family), fm->height()), option.rect.bottomRight());
		QRect border_rect = rect;

		int double_margin = font_family_name_margin*2;

		rect.translate(-font_family_name_margin, -font_family_name_margin);
		border_rect.translate(-double_margin-1, -double_margin-1);
		border_rect.setSize(rect.size() + QSize(double_margin, double_margin));

		painter->setBrush(QBrush(QColor(170, 255, 240)));
		painter->drawRect(border_rect);
		painter->drawText(rect, font_family);
	}

}
