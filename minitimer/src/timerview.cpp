#include "timerview.h"

/** QT headers **/
#include <QTime>
#include <QPainter>
#include <QMenu>
#include <QApplication>

/** Application headers **/
#include "timerdialog.h"

const int WIDGET_BORDER = 1;
const int DRAG_HANDLE_SIZE = 20;

namespace MiniTimer {

	TimerView::TimerView(QWidget *parent):
		QWidget(parent),
		timer(NULL),
		m_dragging(false),
		m_resizing(false) {
		setWindowFlags(Qt::FramelessWindowHint);
		setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		resize(200,200);
	}

	TimerView::~TimerView() {
	}

	void TimerView::paintEvent(QPaintEvent* /* event */) {

		QPainter painter(this);

		int widget_height = height();
		int time_left_height = 0;

		if (timer->time() > 0)
			time_left_height = widget_height * timer->timeLeft() / timer->time();

		int height_passed = widget_height - WIDGET_BORDER * 2 - time_left_height;

		// Widget border;
		painter.setBrush(QBrush(QColor(255,255,255)));
		painter.drawRect(1, 1, width() - WIDGET_BORDER*3, height()-WIDGET_BORDER*3);
		painter.save();

		// time passed
		painter.translate(1, 1);
		painter.setBrush(QBrush(QColor(0,0,0)));
		painter.setPen(QPen(QColor(0,0,0)));
		painter.drawRect(0, 0, width()-WIDGET_BORDER*3, height_passed);

		// time left
		painter.translate(0, height_passed);
		painter.setBrush(QBrush(QColor(255,255,255)));
		painter.setPen(QPen(QColor(255,255,255)));
		painter.drawRect(0, 0, width()-WIDGET_BORDER*3, time_left_height-WIDGET_BORDER);

		painter.restore();

		// Pause sign if paused
		if (timer->paused())
			drawPauseSign(&painter);

		// resize handle
		painter.setBrush(QBrush(QColor(Qt::gray)));
		painter.translate(width(), height());

		static const QPointF resize_handle[] = {
			QPointF(0, 0),
			QPointF(0, -DRAG_HANDLE_SIZE),
			QPointF(-DRAG_HANDLE_SIZE, 0)
		};

		painter.drawPolygon(resize_handle, 3);
	}

}

 void MiniTimer::TimerView::setTimer(Timer *p_timer) {
	this->timer = p_timer;
	connect(this->timer, SIGNAL(tick(int)), SLOT(update()));
}

void MiniTimer::TimerView::mousePressEvent(QMouseEvent *event) {

	click_point = event->pos();
	initial_size = size();

	if (pointInResizeHandle(click_point))
		m_resizing = true;
	else
		m_dragging = true;
}

void MiniTimer::TimerView::mouseReleaseEvent(QMouseEvent* /* event */) {
	m_dragging = false;
	m_resizing = false;
}

void MiniTimer::TimerView::mouseMoveEvent(QMouseEvent *event) {

	QPoint delta;

	if (m_dragging) {
		delta = event->globalPos() - click_point;
		move(delta);
	} else if (m_resizing) {
		delta = event->pos() - click_point;
		resize(initial_size.width() + delta.x(), initial_size.height() + delta.y());
	}

}

bool MiniTimer::TimerView::pointInResizeHandle(QPoint clicked) const {
	return ((clicked.x() > width()-DRAG_HANDLE_SIZE) and (clicked.y() > height()-DRAG_HANDLE_SIZE));
}

void MiniTimer::TimerView::saveState(QSettings *settings) {
	settings->beginGroup("Timer");
	settings->setValue("position", pos());
	settings->setValue("size", size());
	settings->endGroup();
}

void MiniTimer::TimerView::loadState(QSettings *settings) {
	settings->beginGroup("Timer");
	move(settings->value("position", pos()).toPoint());
	resize(settings->value("size", size()).toSize());
	settings->endGroup();
}

void MiniTimer::TimerView::contextMenuEvent(QContextMenuEvent *event) {
	QMenu menu;

	QAction *action;
	action = menu.addAction(tr("Set time"));
	connect(action, SIGNAL(triggered()), SLOT(showTimeDialog()));

	action = menu.addAction(tr("Close"));
	connect(action, SIGNAL(triggered()), SLOT(quit()));

	pause_action = menu.addAction(tr("Pause"));
	pause_action->setCheckable(true);
	connect(pause_action, SIGNAL(triggered()), SLOT(pause()));

	menu.exec(event->globalPos());
}

void MiniTimer::TimerView::quit() {
	QApplication::quit();
}

void MiniTimer::TimerView::pause() {
	timer->pause();
	pause_action->setChecked(timer->paused());
	update();
}

void MiniTimer::TimerView::showTimeDialog() {
	TimerDialog dialog;
	int result = dialog.exec();
	if (result == QDialog::Accepted) {
		timer->stop();
		timer->setTime(dialog.hours(), dialog.minutes(), dialog.seconds());
		timer->start();
	}
}

void MiniTimer::TimerView::drawPauseSign(QPainter *painter) {
	painter->save();
	painter->setBrush(QBrush(QColor(0,255,0)));
	painter->drawRect(10, 10, 10, 20);
	painter->drawRect(25, 10, 10, 20);
	painter->restore();
}
