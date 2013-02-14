#include "settings_dialog.h"

#include <QFileDialog>

namespace FontMate {

	SettingsDialog::SettingsDialog(Settings *settings, QWidget* parent, Qt::WFlags fl):
		QDialog(parent, fl),
		Ui::SettingsDialog(),
		m_settings(settings) {
		setupUi(this);

		connect(this, SIGNAL(accepted()), SLOT(save()));
		connect(browseSaveFontFolderButton, SIGNAL(clicked()), SLOT(selectFontBackupFolder()));
		load();
	}

	SettingsDialog::~SettingsDialog() {
	}

	void SettingsDialog::selectFontBackupFolder() {
		QString path = QFileDialog::getExistingDirectory(this, tr("Select folder to backup uninstalled fonts to"));
		if (not path.isEmpty())
			saveFontFolderLineEdit->setText(QDir(QApplication::applicationDirPath()).relativeFilePath(path));
	}

	void SettingsDialog::save() {
		settings()->setSaveUninstalledFonts(saveUninstalledCheckbox->isChecked());
		settings()->setOverwriteInstalledFonts(overwriteExistingCheckbox->isChecked());
		settings()->setLogging(loggingCheckBox->isChecked());
		settings()->setPreviewHighlighted(previewHighlightedCheckBox->isChecked());

		settings()->setFontBackupFolder(QDir(QApplication::applicationDirPath()).relativeFilePath(saveFontFolderLineEdit->text()));

		if (restoreInstructionsCheckbox->isChecked())
			settings()->restoreDefaultText();
		// TODO : notify ui that default text was restored.
		settings()->setLanguage(settings()->languages->valueAt(languageComboBox->currentIndex()));

		settings()->sync();
	}

	void SettingsDialog::load() {
		saveUninstalledCheckbox->setChecked(settings()->saveUninstalledFonts());
		overwriteExistingCheckbox->setChecked(settings()->overwriteInstalledFonts());
		saveFontFolderLineEdit->setText(settings()->fontBackupFolder());
		loggingCheckBox->setChecked(settings()->logging());
		previewHighlightedCheckBox->setChecked(settings()->previewHighlighted());

		languageComboBox->setModel(settings()->languages);
		int language_index = settings()->languages->indexOfValue(settings()->language());
		if (language_index < 0) language_index = 0;
		languageComboBox->setCurrentIndex(language_index);

	}

	void SettingsDialog::changeEvent(QEvent *e) {
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
