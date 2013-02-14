#pragma once

/** QT headers **/
#include <QStringList>
#include <QTextDocument>
#include <QDesktopServices>
#include <QDir>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** */
#include <stdlib.h>

using namespace JB;

namespace FontMate {

	const QStringList systemFontPaths = QStringList()
			<< QDir::fromNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::FontsLocation))
			<< QDir::fromNativeSeparators(Path::join(getenv("windir"), "Fonts"));

	const QStringList fontFileFilter = QStringList() << "*.ttf" << "*.otf";

	void removeSystemFontPaths(QStringList &paths);

	/**
	  Get font full name. This font name is used by windows to register a font.

	  @param file_path Full path to font file.
	  @return Full font name.
	*/
	QString fontName(QString file_path);

	/**
		Install TrueType font.

		@param path full path to font file.
		@return Path of installed font or empty string on failure.
	*/
	QString installFont(const QString &path);

	/**
		Uninstall TrueType font.

		@param path full path to font file.
		@return True if sucessfull
	*/
	bool uninstallFont(const QString &path);

	/**
		Get the path to install the given font file to.

		@return Path where font file will be copied to when installing.
	*/
	QString fontInstallDestinationPath(const QString &source_font_path);

	/**
		Find the installed font file in the system paths.

		@param font_path Path of font file to search for (can be full or only file name)
		@return Full path to installed font file, or empty string if not found.
	*/
	QString installedFontPath(const QString &font_path);

	/**
		Get the path to backup uninstalled font to.

		@param font_path path of font to be uninstalled.
		@return path to backed up font file.
	*/
	QString fontBackupPath(const QString &font_path);

	/**
	  Check if font file exists in the known system font paths.

	  @param font_path full path to font file (installed or uninstalled)
	  @return True if font file is installed.
	*/
	bool fontInSystemPath(const QString &font_path);

	/**
		Load font into QT font database.

		@param path full path to font file.
		@return id of loaded font family. -1 on failure
	*/
	int loadFont(const QString &path);

	bool folderContainsFonts(const QString &path);

	QString fontSample(QString font_family, QString sample_text, int size);

	QTextDocument* createFontListDocument(const QStringList &families, QString sample_text=QString(), int size=16);

	int fontCount(QString path);

	bool renameFontWithFamilyName(QString font_path);
}
