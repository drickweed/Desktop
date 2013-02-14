#pragma once

/** QT headers **/
#include <QTableView>

class FileTypeReportView : public QTableView {
	Q_OBJECT
	public:
		FileTypeReportView(QWidget *parent);

	signals:
		void filterRequest(QString filter);

	public slots:

	private:
		QString selected_extension;

	protected slots:
		void contextMenu(const QPoint & point);
		void createFilter();

};

