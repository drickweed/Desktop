#include "statussummaryitem.h"

/** QT headers **/
#include <QFont>
#include <QPainter>

namespace MiniSFV {

	StatusSummaryItem::StatusSummaryItem(QWidget *parent):
		QWidget(parent),
		m_count(0) {

		QFont font = checkbox.font();
		font.setWeight(QFont::Bold);
		checkbox.setFont(font);
		label.setFont(font);

		useCheckbox();

		layout = new QHBoxLayout();
		layout->addWidget(&checkbox);
		layout->addWidget(&label);
		setLayout(layout);

		connect(&checkbox, SIGNAL(stateChanged(int)), SIGNAL(stateChanged(int)));
	}

	bool StatusSummaryItem::isChecked() {
		return checkbox.isChecked();
	}

	uint StatusSummaryItem::count() {
		return m_count;
	}

	void StatusSummaryItem::setCount(uint count) {
		if (m_count != count)
			emit countChanged();
		m_count = count;

		updateLabel();
	}

	void StatusSummaryItem::setLabel(QString text) {
		m_label_text = text;
		updateLabel();
	}

	void StatusSummaryItem::select() {
		checkbox.setChecked(true);
	}

	void StatusSummaryItem::unSelect() {
		checkbox.setChecked(false);
	}

	void StatusSummaryItem::paintEvent(QPaintEvent *event) {

		QPainter painter;

		painter.begin(this);
		painter.setBrush(QBrush(color()));
		painter.drawRect(QRect(event->rect().topLeft(), QSize(event->rect().width()-1, event->rect().height()-1)));
		painter.end();

		QWidget::paintEvent(event);
	}

	void StatusSummaryItem::setColor(QColor color) {
		m_color = color;
		update();
	}

	void StatusSummaryItem::updateLabel() {
		QString message = QString("%1 [%2 Files]").arg(m_label_text).arg(count());
		checkbox.setText(message);
		label.setText(message);
	}

	void StatusSummaryItem::useCheckbox() {
		checkbox.show();
		label.hide();
	}

	void StatusSummaryItem::useLabel() {
		checkbox.hide();
		label.show();
	}

}
