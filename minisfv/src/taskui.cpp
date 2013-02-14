/** Application headers **/
#include "taskui.h"

/** QT headers **/
#include <QFileDialog>
#include <QStringList>
#include <QPixmap>
#include <QDebug>

namespace MiniSFV {

	TaskUI::TaskUI(QWidget *parent):
		QWidget(parent) {

		create_btn = new QPushButton(tr("Create"), this);
		create_btn->setToolTip(tr("Create checksum file"));
		create_btn->hide();

		verify_btn = new QPushButton(tr("Check"), this);
		verify_btn->setToolTip(tr("Verify files"));
		verify_btn->hide();

		select_files_btn = new QPushButton(QPixmap(":/images/add.png"),"");
		select_files_btn->setToolTip(tr("Select files or load sfv file"));
		select_files_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

		browser_registration_btn = new QPushButton();
		updateRegisterInBrowserButtonText();

		layout = new QHBoxLayout(this);
		layout->addWidget(select_files_btn);
		layout->addWidget(create_btn);
		layout->addWidget(verify_btn);
		layout->addStretch(1);
		layout->addWidget(browser_registration_btn);
		setLayout(layout);

		connect(create_btn, SIGNAL(clicked()), SIGNAL(create()));
		connect(verify_btn, SIGNAL(clicked()), SIGNAL(verify()));
		connect(select_files_btn, SIGNAL(clicked()), SLOT(selectFiles()));
		connect(browser_registration_btn, SIGNAL(clicked()), SLOT(changeBrowserRegistration()));
	}

	void TaskUI::showVerify() {
		verify_btn->show();
	}

	bool TaskUI::isRegisteredInBrowser() const {
		return false;
	}

	void TaskUI::selectFiles() {
		QStringList files = QFileDialog::getOpenFileNames(this, "Select checksum file or files to generate checksum file for", "", "All files (*)");

		emit filesSelected(files);
	}

	void TaskUI::changeBrowserRegistration() {
		if (isRegisteredInBrowser())
			unregisterFromBrowser();
		else
			registerInBrowser();
	}

	void TaskUI::updateCreateActionAvailability(int record_count) {
		create_btn->setHidden(record_count == 0);

	}

	void TaskUI::registerInBrowser() {

	}

	void TaskUI::unregisterFromBrowser() {

	}

	void TaskUI::updateRegisterInBrowserButtonText() {
		if (isRegisteredInBrowser())
			browser_registration_btn->setText(tr("Unregister from browser"));
		else
			browser_registration_btn->setText(tr("Register in browser"));
	}

}
