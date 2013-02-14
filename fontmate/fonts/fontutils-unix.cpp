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

/** Application headers **/
#include <settings/settings.h>

using namespace JB;

namespace FontMate {

	QString installFont(const QString &path) {

		QString destination = fontInstallDestinationPath(path);
		qDebug() << "Installing font to" << destination;

		if (QFile(destination).exists()) {
			if (Settings().overwriteInstalledFonts()) {// Remove existing font file.
				if (not QFile(destination).remove()) {
					qDebug() << "Error deleting font file" << destination;
					return QString();
				}

				if (not QFile(path).copy(destination)) {
					qDebug() << "Error copying font file to" << destination;
					return QString();
				}
			}
		} else {
			if (not QFile(path).copy(destination)) {
				qDebug() << "Error copying font file to" << destination;
				return QString();
			}
		}
	   return path;
	}

	bool uninstallFont(const QString &file_path) {

		qDebug() << "Uninstalling font" << file_path;

		if (file_path.isEmpty())
			return true;

		if (not QFileInfo(file_path).exists())
			return true;
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
