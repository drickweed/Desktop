#pragma once

#include <QDialog>

#include "ui_manual_time_select.h"

namespace MiniTimer {

	class TimerDialog : public QDialog, private Ui::Dialog {
		Q_OBJECT

		public:
			TimerDialog(QWidget* parent=0, Qt::WFlags fl=0);

			virtual ~TimerDialog();

			int hours() const { return hoursSpinBox->value(); }
			int minutes() const { return minutesSpinBox->value(); }
			int seconds() const { return secondsSpinBox->value(); }

		protected slots:

			virtual void reject();

			virtual void accept();

	};

}
