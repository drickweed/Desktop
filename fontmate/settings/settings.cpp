#include "settings.h"

/** QT headers **/
#include <QVariant>
#include <QApplication>

/** JB headers **/
#include <jb/qt4/pathutils.h>

using namespace JB;

namespace FontMate {

	Settings::Settings(QObject *parent):
		QObject(parent) {
		settings = new QSettings(configFile(), QSettings::IniFormat, this);
		languages = new JB::OptionsModel(this);
		languages->setDisplayItems(QStringList() << "English" << "Hebrew");
		languages->setValueItems(QStringList() << "en" << "he");
	}

	Settings::~Settings() {
		settings->sync();
		delete settings;
	}

	QString Settings::defaultText() {
		return settings->value("default/text", QVariant("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Adobe Caslon Pro Bold'; font-size:14pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">This text remains in sync with all other preview boxes with regards to content and format, except for the selected font family.</p><p><ul><li>Edit this text using the tools above.</li><li>Use the mouse wheel while pressing Ctrl to enlarge or shrink the entire text.</li><li>The font family name appears at the bottom right.</li><li>Select fonts using the checkbox next to their name, and then select an action from the menubar.</li><li>By default uninstall fonts are saved in 'uninstalled_fonts' folder</li><li>Additional options available in the settings dialog.</li></ul><p></body></html>")).toString();
	}

	void Settings::setDefaultText(QString text) {
		settings->setValue("default/text", QVariant(text));
	}

	QString Settings::configFile() const {
		return QString("%1/%2").arg(qApp->applicationDirPath(), "fontmate.ini");
	}

	void Settings::sync() {
		settings->sync();
	}

	int Settings::defaultSize() {
		return settings->value("default/size", QVariant(14)).toInt();
	}

	void Settings::setDefaultSize(int size) {
		settings->setValue("default/size", QVariant(size));
	}

	QString Settings::language() {
		return settings->value("language", QVariant("en")).toString();
	}

	void Settings::setLanguage(QString language) {
		settings->setValue("language", QVariant(language));
		emit languageChanged(language);
	}

	QString Settings::translationFile() {
		return QString("fontmate_%1.qm").arg(language());
	}

	QStringList Settings::fontPaths() {
		return settings->value("font_paths", QVariant(QStringList())).toStringList();
	}

	void Settings::setFontPaths(QStringList paths) {
		settings->setValue("font_paths", QVariant(paths));
	}

	int Settings::listFontSize() {
		return settings->value("list_preview_size", QVariant(12)).toInt();
	}

	void Settings::setListFontSize(int size) {
		settings->setValue("list_preview_size", QVariant(size));
	}

	void Settings::setDonated(bool donated) {
		settings->setValue("donated", QVariant(donated));
		emit this->donated(donated);
	}

	bool Settings::donated() {
		return settings->value("donated", QVariant(false)).toBool();
	}

	bool Settings::saveUninstalledFonts() {
		return settings->value("save_uninstalled", QVariant(true)).toBool();
	}

	QString Settings::fontBackupFolder() {
		return settings->value("backup_folder", QVariant("uninstalled_fonts")).toString();
	}

	bool Settings::overwriteInstalledFonts() {
		return settings->value("overwrite_installed", QVariant(true)).toBool();
	}

	void Settings::setSaveUninstalledFonts(bool save) {
		settings->setValue("save_uninstalled", QVariant(save));
	}

	void Settings::setOverwriteInstalledFonts(bool overwrite) {
		settings->setValue("overwrite_installed", QVariant(overwrite));
	}

	void Settings::setFontBackupFolder(QString folder) {
		settings->setValue("backup_folder", QVariant(folder));
	}

	void Settings::restoreDefaultText() {
		settings->remove("default/text");
	}

	bool Settings::logging() {
		return settings->value("logging", QVariant(false)).toBool();
	}

	void Settings::setLogging(bool enabled){
		settings->setValue("logging", QVariant(enabled));
	}

	bool Settings::previewHighlighted() {
		return settings->value("preview_highlighted", QVariant(true)).toBool();
	}

	void Settings::setPreviewHighlighted(bool enabled) {
		settings->setValue("preview_highlighted", QVariant(enabled));
	}
}
