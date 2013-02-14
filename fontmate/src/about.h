#pragma once

#include <QDialog>
#include "ui_about.h"

namespace FontMate {

	class About : public QDialog, private Ui::Dialog {
		Q_OBJECT

		public:
			enum Result {DonateLater, Donate, AlreadyDonated};

			About(QWidget* parent = 0, Qt::WFlags fl = 0 );
			~About();

		private slots:
			void donate();
			void alreadyDonated();
			void donateLater();

			void openSite();

		protected:

		protected slots:

	};

}
