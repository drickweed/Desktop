#pragma once

#include <QDialog>

#include "settings.h"

#include "ui_settings.h"

namespace FontMate {

	class SettingsDialog : public QDialog, private Ui::SettingsDialog {
		Q_OBJECT

		public:
			SettingsDialog(Settings *settings, QWidget *parent=0, Qt::WFlags fl=0);
			~SettingsDialog();

		private:
			Settings *m_settings;

			Settings *settings() const { return m_settings; }

		private slots:
			void save();
			void load();
			void selectFontBackupFolder();

		protected:
			void changeEvent(QEvent *e);

		protected slots:

	};

}
