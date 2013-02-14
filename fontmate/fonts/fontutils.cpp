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

// Type definitions and macros
#define tag_NamingTable         0x656d616e        /* 'name' */

// Macros for TrueType portability
#define FS_2BYTE(p)  ( ((unsigned short)((p)[0]) << 8) |  (p)[1])
#define FS_4BYTE(p)  ( FS_2BYTE((p)+2) | ( (FS_2BYTE(p)+0L) << 16) )
#define SWAPW(a)        ((short) FS_2BYTE( (unsigned char *)(&a) ))
#define SWAPL(a)        ((long) FS_4BYTE( (unsigned char *)(&a) ))
#define OFFSETTABLESIZE     12  /* not including any entries */

typedef short int16;
typedef unsigned short uint16;
typedef long int32;
typedef unsigned long uint32;
typedef long sfnt_TableTag;

typedef struct {
	uint16 platformID;
	uint16 specificID;
	uint16 languageID;
	uint16 nameID;
	uint16 length;
	uint16 offset;
} sfnt_NameRecord;

typedef struct {
	uint16 format;
	uint16 count;
	uint16 stringOffset;
} sfnt_NamingTable;

typedef struct {
	sfnt_TableTag   tag;
	uint32          checkSum;
	uint32          offset;
	uint32          length;
} sfnt_DirectoryEntry;

typedef struct {
	int32 version;                  /* 0x10000 (1.0) */
	uint16 numOffsets;              /* number of tables */
	uint16 searchRange;             /* (max2 <= numOffsets)*16 */
	uint16 entrySelector;           /* log2 (max2 <= numOffsets) */
	uint16 rangeShift;              /* numOffsets*16-searchRange*/
	sfnt_DirectoryEntry table[1];   /* table[numOffsets] */
} sfnt_OffsetTable;


using namespace JB;

namespace FontMate {

	QString fontName(QString file_path) {
		QString font_full_name;
		char		      namebuf[255];
		unsigned short      numNames;
		long		      curseek;
		sfnt_OffsetTable    OffsetTable;
		sfnt_DirectoryEntry Table;
		sfnt_NamingTable    NamingTable;
		sfnt_NameRecord     NameRecord;

		QFile font_file(file_path);

		if (not font_file.open(QIODevice::ReadOnly))
			return QString();

		/* First off, read the initial directory header on the TTF.  We're only
		   * interested in the "numOffsets" variable to tell us how many tables
		   * are present in this file.
		   *
		   * Remember to always convert from Motorola format (Big Endian to
		   * Little Endian).
		   */
		font_file.read((char*) &OffsetTable, sizeof (OffsetTable) - sizeof (sfnt_DirectoryEntry));

		unsigned int cTables = (int) SWAPW (OffsetTable.numOffsets);

		cTables = qMin(cTables, (unsigned int)40);

		for ( unsigned int i=0; i < cTables; i++) {
			if ((font_file.read((char*)&Table, sizeof (Table))) != sizeof(Table))
				break;
			if (Table.tag == tag_NamingTable) {
			/* Now that we've found the entry for the name table, seek to that
			 * position in the file and read in the initial header for this
			 * particular table.  See "True Type Font Files" for information
			 * on this record layout.
			 */
				font_file.seek(SWAPL(Table.offset));
				font_file.read((char*)&NamingTable, sizeof (NamingTable));
				numNames = SWAPW(NamingTable.count);
				while (numNames--) {
					font_file.read((char*)&NameRecord, sizeof(NameRecord));
					curseek = font_file.pos();
					if (SWAPW(NameRecord.platformID) == 1 &&
						SWAPW(NameRecord.nameID) == 4) {
						font_file.seek(SWAPW (NameRecord.offset) +
									   SWAPW(NamingTable.stringOffset) +
									   SWAPL(Table.offset));
						font_file.read((char*)&namebuf, SWAPW(NameRecord.length));
						namebuf[SWAPW(NameRecord.length)] = '\0';
						font_full_name = namebuf;
						font_file.seek(curseek);
					}
				}
			}
		}
		return font_full_name;
	}

	bool renameFontWithFamilyName(QString font_path) {
		QString new_font_path;
		QString extension = QFileInfo(font_path).suffix();
		QString family_name = fontName(font_path);
		family_name.replace('\\',' ');

		new_font_path = QString("%1.%2").arg(family_name, extension);
		new_font_path = JB::Path::join(QFileInfo(font_path).absolutePath(), new_font_path);

		LOG("Renaming:ACTION " << font_path.toStdString() << " -> " << new_font_path.toStdString());
		bool result = QFile(font_path).rename(new_font_path);

		if (not result)
			LOG("Renaming:ERROR Failed");

		return result;
	}

	QString fontBackupPath(const QString &font_path) {

		QString backup_path = Settings().fontBackupFolder();

		if (not QFileInfo(backup_path).isAbsolute())
			backup_path = Path::join(QApplication::applicationDirPath(), backup_path);

		return Path::join(backup_path, QFileInfo(font_path).fileName());

	}

	QString fontInstallDestinationPath(const QString &source_font_path) {
		QString destination(
				QDir::toNativeSeparators(Path::join(
				QDesktopServices::storageLocation(QDesktopServices::FontsLocation),
				QFileInfo(source_font_path).fileName()))
		);

		return destination;
	}

	QString installedFontPath(const QString &font_path) {

		QString font_file = QFileInfo(font_path).fileName();

		foreach(QString system_font_path, systemFontPaths) {
			QString	full_path = Path::join(system_font_path, font_file);
			if (QFile(full_path).exists())
				return full_path;
		}

		return QString();
	}

	bool fontInSystemPath(const QString &font_path) {
		return (not installedFontPath(font_path).isEmpty());
	}

	int loadFont(const QString &file) {
		LOG("Loading font " << file.toStdString());
		try {
			int id = QFontDatabase::addApplicationFont(file);
			return id;
		} catch (std::exception *e) {
			LOG("Error loading font");
			return -1;
		}
	}

	bool folderContainsFonts(const QString &path) {
		QDir dir(path);
		return (dir.exists() and (not dir.entryList(fontFileFilter).empty()));
	}

	QTextDocument* createFontListDocument(const QStringList &families, QString sample_text, int size) {
		QTextDocument *print_document = new QTextDocument;
		QTextCursor cursor = print_document->rootFrame()->firstCursorPosition();
		QString html_output;

		html_output.append("<table border=\"1\" style=\"border: 1pt solid black;\">");

		foreach(QString family, families) {
			// font family name
			if (not sample_text.isEmpty())
				html_output.append(fontSample(family, sample_text, size));
			else
				html_output.append(fontSample(family, family, size));
		}
		html_output.append("</table>");

		cursor.insertBlock();
		cursor.insertHtml(html_output);

		return print_document;
	}

	QString fontSample(QString font_family, QString sample_text, int size) {
		return QString("<tr><td style=\"font-family:Arial; vertical-align:middle; padding-left:1em;\">%1 </td><td style=\" font-family: %1; font-size:%3pt; padding-left:5px;\">%2</td></tr>").arg(font_family, sample_text).arg(size);
	}

	void removeSystemFontPaths(QStringList &paths) {
		for(int i=0; i < paths.count(); i++)
			if (systemFontPaths.contains(paths[i], Qt::CaseInsensitive)) {
				paths.removeAt(i);
				i--;
		}
	}

	int fontCount(QString path) {
		QDir dir(path);
		dir.setNameFilters(fontFileFilter);
		return dir.entryList().count();
	}
}
