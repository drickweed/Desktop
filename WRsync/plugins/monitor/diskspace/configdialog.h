#pragma once

/** QT headers **/
#include <QSettings>

/** Application headers **/
#include "ui_configdialog.h"
#include "settings.h"

namespace DiskSpace {

	class ConfigDialog : public QWidget, public Ui::ConfigDialog {
		Q_OBJECT
		public:
			ConfigDialog(QWidget *parent = 0);

			void saveConfig(DiskSpace::Settings *settings);
			void loadConfig(DiskSpace::Settings *settings);

		protected:
			void changeEvent(QEvent *e);
	};

}
