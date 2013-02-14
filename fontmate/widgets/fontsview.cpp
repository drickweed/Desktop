#include "fontsview.h"

/** QT headers **/
#include <QPainter>
#include <QHeaderView>

/** Application headers **/
#include <models/fontfilter.h>
#include <settings/settings.h>

const int fonts_message_border = 10;

namespace FontMate {

	FontsView::FontsView(QWidget *parent):
		QTableView(parent) {
		setSelectionBehavior(QAbstractItemView::SelectRows);
		setSelectionMode(QAbstractItemView::SingleSelection);
		font_view = new FontItemDelegate(this);
	}

	FontsView::~FontsView() {
	}

	Fonts* FontsView::fonts() const {
		if (not fontsProxy())
			return NULL;
		return qobject_cast<Fonts*>(fontsProxy()->sourceModel());
	}

	void FontsView::paintEvent(QPaintEvent *event) {
		QTableView::paintEvent(event);

		QPainter painter(this->viewport());
		QFont message_font = font();
		message_font.setPointSize(20);
		painter.setFont(message_font);
		QRect message_box(fonts_message_border, fonts_message_border, width() - fonts_message_border*2, height() - fonts_message_border*2);

		if (not fonts())
			return;

		if (fonts()->collectionSource() == Fonts::None) {
			painter.drawText(message_box, "No font collection selected. Select one from the list above.");
			return;
		}

		if ((fonts()->empty()) and (fonts()->areLoading())) {
			painter.drawText(message_box, "Loading fonts...");
			return;
		}

		if ((fonts()->empty()) and (not fonts()->areLoading())) {
			painter.drawText(message_box, "No fonts found in folder.");
			return;
		}

		if ((fontsProxy()->empty()) and (not fontsProxy()->searchText().isEmpty())) {
			painter.drawText(message_box, "No search results.");
			return;
		}
	}

	void FontsView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {

		QTableView::selectionChanged(selected, deselected);

		if (not fonts())
			return;

		if (not fontsProxy())
			return;

		if (selected.indexes().empty())
			return;

		QModelIndex index = selected.indexes()[0];

		if (not index.isValid())
			return;

		if (fontsProxy()->showMoreItem(index)) {
			fontsProxy()->showMoreItems();
			return;
		}

		int id = fontsProxy()->mapToSource(index).row();

		if (Settings().previewHighlighted()) {
			if (fonts()->selected(id))
				emit selectedFontHighlighted(fonts()->family(id));
			else
				emit unselectedFontHighlighted(fonts()->family(id));
		}
	}

	void FontsView::setModel(QAbstractItemModel *model) {
		QTableView::setModel(model);
		setItemDelegate(font_view);

		FontFilter *font_filter_proxy = qobject_cast<FontFilter*>(model);
		if (font_filter_proxy) {
			font_view->fonts_model = qobject_cast<Fonts*>(font_filter_proxy->sourceModel());
			font_view->proxy_model = font_filter_proxy;
		}

		verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		//horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		horizontalHeader()->setResizeMode(QHeaderView::Stretch);
		horizontalHeader()->hide();
		verticalHeader()->hide();
	}

}
