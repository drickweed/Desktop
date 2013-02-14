#pragma once

/** QT headers **/
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QString>

namespace MiniSFV {

	/**
		Task buttons.

		- Add files
		- Create checksum file
		- Verify files
		- (un)Register in file browser

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class TaskUI : public QWidget {
		Q_OBJECT
		public:
			/**
				Construcor.

				@param parent Parent QObject
			*/
			TaskUI(QWidget *parent=0);

		signals:
			/**
				Verify action.
			*/
			void verify(); /// Verify checksums

			/**
				Create checksums file action.
			*/
			void create();

			void filesSelected(QStringList &files);

		private:
			QPushButton *verify_btn, ///> Verify checksums
						*create_btn; ///> Create checksums
			QHBoxLayout *layout; ///> Top layout

			QPushButton *select_files_btn,
						*browser_registration_btn;

			bool isRegisteredInBrowser() const;

			void registerInBrowser();

			void unregisterFromBrowser();


		public slots:
			/**
				Show verification action button.
			*/
			void showVerify();

			/**
				Update availability of create checksum file action according to number of records given.

				@param record_count number of records in file list model.
			*/
			void updateCreateActionAvailability(int record_count);

		private slots:

			void changeBrowserRegistration();
			void selectFiles();
			void updateRegisterInBrowserButtonText();
	};
}

