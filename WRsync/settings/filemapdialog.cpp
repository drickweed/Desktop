#include "filemapdialog.h"

FileMapDialog::FileMapDialog(QWidget *parent) :
	QWidget(parent){
	setupUi(this);

	file_type_report_model = new FileTypeReportModel(this);
	connect(refreshButton, SIGNAL(clicked()), SLOT(refresh()));

	file_type_report_view = new FileTypeReportView(this);
	fileTypeReportLayout->addWidget(file_type_report_view);

	file_type_report_view->setModel(file_type_report_model);
}

void FileMapDialog::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

void FileMapDialog::setSources(QStringList sources) {
	file_type_report_model->setSources(sources);
}

void FileMapDialog::refresh() {
	file_type_report_model->refresh();
}
