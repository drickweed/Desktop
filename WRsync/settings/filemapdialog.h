#pragma once

#include "ui_filemapdialog.h"
#include "filetypereportmodel.h"
#include "filetypereportview.h"

class FileMapDialog : public QWidget, private Ui::FileMapDialog {
	Q_OBJECT
	public:
		FileMapDialog(QWidget *parent = 0);

		void setSources(QStringList sources);

	public slots:
		void refresh();

	private:
		FileTypeReportModel *file_type_report_model;
		FileTypeReportView *file_type_report_view;

	protected:
		void changeEvent(QEvent *e);
};

