#include "windowsfontregistry.h"

/** QT headers **/
#include <QFileInfo>

/** WINDOWS headers **/
#ifdef Q_OS_WIN32
#include <windows.h>
#include <fcntl.h>
#endif

/** Application headers **/
#include <settings/settings.h>
#include "fonts/fontutils.h"

namespace FontMate {

	QMap<QString, QString> WindowsFontRegistry::registry_map;
	QMap<QString, QString> WindowsFontRegistry::registry_full_path;
	QMap<QString, QString> WindowsFontRegistry::registry_full_path_by_filename;

	WindowsFontRegistry::WindowsFontRegistry() {
	}

	void WindowsFontRegistry::clear() {
		registry_map.clear();
		registry_full_path.clear();
		registry_full_path_by_filename.clear();
	}

	void WindowsFontRegistry::load() {
		clear();

		QString file;
		#ifdef Q_OS_WIN32
		QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", QSettings::NativeFormat);
		foreach(QString registry_font_title, settings.childKeys()) {
			file = settings.value(registry_font_title).toString();

			if (QFileInfo(file).isAbsolute()) {
				registry_map[registry_font_title] = file;
				registry_full_path[registry_font_title] = file;
				registry_full_path_by_filename[QFileInfo(file).fileName()] = file;
				continue;
			}

			QString full_path = installedFontPath(file);
			if (full_path.isEmpty())
				continue;

			registry_map[registry_font_title] = file;
			registry_full_path[registry_font_title] = full_path;
			registry_full_path_by_filename[QFileInfo(file).fileName()] = full_path;

		}
		#endif
	}

	QString WindowsFontRegistry::registeredFontTitle(const QString &installed_font_full_path) {
		return registry_full_path.key(installed_font_full_path);
	}

	QStringList WindowsFontRegistry::registeredFontRealPaths() {
		return registry_full_path.values();
	}

	QStringList WindowsFontRegistry::registeredFontFiles() {
		return registry_map.values();
	}

	QStringList WindowsFontRegistry::registeredFontFileNames() {
		return registry_full_path_by_filename.keys();
	}

	void WindowsFontRegistry::notifyChanges() {
		#ifdef Q_OS_WIN32
		// qDebug() << "Notifying windows of font changes";
		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0, (LPARAM)(LPCSTR)"fonts");
		#endif
	}

	bool WindowsFontRegistry::fontRegistered(const QString &non_installed_font_path) {
		QString font_file = QFileInfo(non_installed_font_path).fileName();
		return registeredFontFileNames().contains(font_file);
	}

}
