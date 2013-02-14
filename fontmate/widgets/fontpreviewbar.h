#pragma once

/** QT headers **/
#include <QWidget>
#include <QPushButton>

#include "ui_fontpreviewbar.h"

namespace FontMate {

	/**
		Font preview sidebar with buttons

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FontPreviewBar : public QWidget, public Ui::FontPreviewBar {
		Q_OBJECT
		public:
			FontPreviewBar(QWidget *parent = 0);

			~FontPreviewBar();

			virtual QSize sizeHint () const;

			bool selected() const { return m_selected;}

			void setSelected(bool selected) {
				m_selected = selected;
				update();
			}

		protected:
			virtual void paintEvent(QPaintEvent *event);

			/** QT Api **/
			void mouseReleaseEvent(QMouseEvent *event);

			bool m_selected;

		private slots:
			void upadteSyncButtonColor();

		signals:
			void syncStateChanged(bool state);
	};

}
