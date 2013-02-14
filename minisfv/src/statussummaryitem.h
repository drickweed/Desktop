#pragma once

/** QT headers **/
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QColor>
#include <QPaintEvent>

namespace MiniSFV {

	/**
		Checkbox and label representing the results of a checksum test.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class StatusSummaryItem : public QWidget {
		Q_OBJECT
		public:
			StatusSummaryItem(QWidget *parent=0);

			bool isChecked();

			uint count();
			void setCount(uint count);

			void select(); /// Select item
			void unSelect(); /// Unselect item

			void setLabel(QString text);

			QCheckBox checkbox;
			QLabel label;

			void setColor(QColor color);

			void useCheckbox();
			void useLabel();

		signals:
			void stateChanged(int);
			void countChanged();

		private:
			uint m_count;

			QHBoxLayout *layout;
			QString m_label_text;
			QColor m_color;

			QColor color() const { return m_color; }

			void updateLabel();

		protected:
			void paintEvent(QPaintEvent *event);
	};

}
