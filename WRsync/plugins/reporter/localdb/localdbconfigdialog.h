#pragma once

/** Application headers **/
#include "ui_localdbconfigdialog.h"
#include "settings.h"

namespace LocalDb {

	class ConfigDialog : public QWidget, private Ui::LocalDbConfigDialog {
		Q_OBJECT
		public:
			ConfigDialog(QWidget *parent = 0);

			void loadSettings(Settings *settings);
			void saveSettings(Settings *settings);

		private slots:
			void showSelectDatabaseDialog();

		protected:
			void changeEvent(QEvent *e);

		signals:
			void resetDatabase();
	};
}
