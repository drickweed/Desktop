#include "window.h"

#include <QMessageBox>

namespace MiniSFV {

	Window::Window(FileChecksumModel *model, QWidget *parent):
			QWidget(parent) {

		controls = new Controls(model, this);

		top_stack = new QStackedLayout();
		top_stack->addWidget(controls);

		setLayout(top_stack);

		setWindowTitle(tr("MiniSFV"));

	}

	void Window::showChecksumFileWriteError(QString filename, QString reason) {
		QString message = QString("Error writing checksum file: %1").arg(filename);
		if (not reason.isEmpty())
			message.append(QString("\nReason: %1").arg(reason));

		QMessageBox::critical(this, tr("Error writing checksum file"), message);
	}

}
