#pragma once

// QT includes
#include <QPushButton>
#include <QHBoxLayout>


namespace MiniSFV {

	/**
		Abort/Skip UI controls.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class AbortUI : public QWidget {
		Q_OBJECT
		public:
			/**
				Constructor.
			*/
			AbortUI(QWidget *parent = 0);

			/**
				Destructor.
			*/
			~AbortUI();

		signals:
			void abort(); /// Abort current operation
			void skip(); /// Skip current operation

		private:
			QPushButton *abort_btn, ///> Abort button
						*skip_btn; ///> Skip button
			QHBoxLayout *layout; ///> Top level layout
	};
}

