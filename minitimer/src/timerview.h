#pragma once

/** QT headers **/
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QSize>
#include <QSettings>
#include <QAction>

/** Application headers **/
#include "timer.h"

namespace MiniTimer {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class TimerView : public QWidget {
		Q_OBJECT
		public:
			TimerView(QWidget *parent=0);

			virtual ~TimerView();

			virtual void paintEvent(QPaintEvent *event);

			void setTimer(Timer *p_timer);

			void loadState(QSettings *settings);

			void saveState(QSettings *settings);

		protected:
			/**
				@internal note: no need for event->accept() on any of these (according to scribble pad example)
			*/

			/** QT Api **/
			void mousePressEvent(QMouseEvent *event);

			/** QT Api **/
			void mouseReleaseEvent(QMouseEvent *event);

			/** QT Api **/
			void mouseMoveEvent(QMouseEvent *event);

			/** QT Api **/
			void contextMenuEvent(QContextMenuEvent *event);

			/**
				Check if resize handle was clicked.
			*/
			bool pointInResizeHandle(QPoint clicked) const;

		private:
			Timer *timer;
			bool m_dragging, m_resizing;
			QPoint click_point; ///< Point mouse was clicked on relative to widget.
			QSize initial_size;

			void drawPauseSign(QPainter *painter);

			QAction *pause_action;

		private slots:
			void showTimeDialog();
			void pause();
			void quit();

	};

}
