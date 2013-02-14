
#include "fontutils.h"

/** QT headers **/
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QFontDatabase>
#include <QApplication>
#include <QTextCursor>
#include <QTextFrame>

/** JB headers **/
#include <jb/qt4/pathutils.h>
#include <jb/qt4/exceptions.h>
#include <jb/std/log.h>

using namespace JB;

/** WINDOWS headers **/
#ifdef Q_OS_WIN32
#include <windows.h>
#include <fcntl.h>
#include <stdlib.h>
#endif

/** Application headers **/
#include <settings/settings.h>
#include <models/windowsfontregistry.h>

namespace FontMate {

	QString installFont(const QString &path) {

		QString destination = fontInstallDestinationPath(path);
		qDebug() << "Installing font to" << destination;

		if (QFile(destination).exists()) {
			if (Settings().overwriteInstalledFonts()) {// Remove existing font file.
				if (RemoveFontResource(destination.toStdWString().c_str()) == 0) {
					qDebug() << "Error unregistering font file" << destination;
				}

				if (not QFile(destination).remove()) {
					qDebug() << "Error deleting font file" << destination;
					return QString();
				}

				if (not QFile(path).copy(destination)) {
					qDebug() << "Error copying font file to" << destination;
					return QString();
				}
			} else {
				qDebug() << "Skipping existing font" << destination;
				return QString();
			}
		} else {
				if (not QFile(path).copy(destination)) {
					qDebug() << "Error copying font file to" << destination;
					return QString();
				}
			}

		if (AddFontResource(destination.toStdWString().c_str()) == 0) {
			qDebug() << "Error registering font in windows system";
			return QString();
		}

		QString file_name = QFileInfo(destination).fileName();
		QString font_registered_name = QString("%1 (TrueType)").arg(fontName(destination));

		qDebug() << "Writing font entry in windows system as" << font_registered_name;
		WriteProfileString(QString("fonts").toStdWString().c_str(),
						   font_registered_name.toStdWString().c_str(),
						   file_name.toStdWString().c_str());

		return path;
	}

	bool uninstallFont(const QString &file_path) {

		qDebug() << "Uninstalling font" << file_path;

		if (file_path.isEmpty())
			return true;

		if (not QFileInfo(file_path).exists())
			return true;

		if (RemoveFontResource(file_path.toStdWString().c_str()) == 0) {
			qDebug() << "Error unregistering font file"  << file_path;
			//return false;
		}

		QString font_registered_name = WindowsFontRegistry::registeredFontTitle(file_path);

		// Clear registry entry
		WriteProfileString((WCHAR*)"fonts",
						   font_registered_name.toStdWString().c_str(),
						   NULL);

		// Must notify here so applications will let go of files.
		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0, (LPARAM)(LPCSTR)"fonts");

		QFile file(file_path);

		if (file.exists()) {

			if (Settings().saveUninstalledFonts()) {
				try {
					Path::ensurePath(Settings().fontBackupFolder());
				} catch (pathEnsureError *e) {
					return false;
				}

				QString backup_file_path = fontBackupPath(file_path);

				qDebug() << "Backing up font file" << file_path << "to" << backup_file_path;
				QFile backup_file(backup_file_path);

				if (backup_file.exists()) {
					if (not backup_file.remove()) {
						qDebug() << "Error removing old backup font file" << backup_file_path;
						return false;
					}
				}

				if (not file.copy(backup_file_path)) {
					qDebug() << "Failed backing up font file to" << backup_file_path;
					return false;
				}
			}

			if (not file.remove())
				qDebug() << "Error deleting font file" << file_path;
				return false;
		}

		return true;
	}
}
