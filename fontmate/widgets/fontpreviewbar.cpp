#include "fontpreviewbar.h"

/** QT headers **/
#include <QPainter>
#include <QApplication>
#include <QVBoxLayout>
#include <QRect>

namespace FontMate {

	FontPreviewBar::FontPreviewBar(QWidget *parent):
		QWidget(parent),
		Ui::FontPreviewBar(),
		m_selected(false) {

		Ui::FontPreviewBar::setupUi(this);
		setFont(QApplication::font());
		upadteSyncButtonColor();

		connect(syncButton, SIGNAL(toggled(bool)), SLOT(upadteSyncButtonColor()));
		connect(syncButton, SIGNAL(toggled(bool)), SIGNAL(syncStateChanged(bool)));

		emit syncStateChanged(syncButton->isChecked());
	}

	FontPreviewBar::~FontPreviewBar() {
	}

	void FontPreviewBar::paintEvent(QPaintEvent * /*event*/) {

		QFont font = QApplication::font();
		QFontMetrics fm(font);

		QPainter painter(this);
// 		painter.drawText(5 , 5+ fm.height(), "X");
	}

	QSize FontPreviewBar::sizeHint() const {
		QFont font = QApplication::font();
		QFontMetrics fm(font);

		return QSize(fm.width("X") + 10, 30);
	}

	void FontPreviewBar::upadteSyncButtonColor() {
		QPalette changed_palette = palette();
		if (syncButton->isChecked())
			changed_palette.setColor(QPalette::Button, Qt::green);
		syncButton->setPalette(changed_palette);
	}

}

void FontMate::FontPreviewBar::mouseReleaseEvent(QMouseEvent * /*event*/) {
	QFont font = QApplication::font();
	QFontMetrics fm(font);

// 	QRect close_button = QSize(fm.width("X") + 10, 30);
}
