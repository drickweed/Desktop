#include "mainwindow.h"

/** JB headers **/
#include <jb/std/log.h>

/** Application headers **/
#include <settings/settings_dialog.h>

using namespace JB;

namespace FontMate {

	const int FONTMATE_STATE_VERSION=0;

	void MainWindow::showSettingsDialog() {
		settings->sync();
		SettingsDialog(settings).exec();
		Logger::Instance().setEnabled(settings->logging());
		if (not settings->previewHighlighted())
			fontPreviews()->removeTemporaryView();
	}

	void MainWindow::saveState() {
		QFile state_file(stateFile());
		state_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		state_file.write(QMainWindow::saveState(FONTMATE_STATE_VERSION));
	}

	void MainWindow::loadState() {
		QFile state_file(stateFile());
		if (not state_file.open(QIODevice::ReadOnly))
			return;

		restoreState(state_file.readAll(), FONTMATE_STATE_VERSION);
	}

	void MainWindow::saveDefaultContent() {
		settings->setDefaultText(fontPreviews()->previews()[0]->editor()->toHtml());
	}

	void MainWindow::loadSettings() {
		listFontSize->setValue(settings->listFontSize());
		fontPreviews()->setDefaultFontSize(settings->defaultSize());
		fontSize->setEditText(QVariant(settings->defaultSize()).toString());
		loadState();
		loadFontPaths();
	}

	void MainWindow::changeEvent(QEvent *e) {
		QWidget::changeEvent(e);
		switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
		}
	}

}
