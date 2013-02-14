#include "create_album.h"

/** QT headers **/
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QSqlRecord>
#include <QProcess>
#include <QDebug>

/** JB headers **/
#include <jb/qt4/pathutils.h>
#include <jb/qt4/app.h>
#include <jb/std/math.h>
#include <jb/std/log.h>

/** Application headers **/
#include "settings/system.h"
#include "settings/export.h"
#include "imp/album.h"
#include "imp/person.h"
#include "imp/picture.h"
#include "storage/get.h"

/**
	Convert any variable QVariant knows about to c-string (char*)

	@param i variable to convert
*/
#define TO_C_STR(i) QVariant(i).toString().toStdString().c_str()

/**
	Write newline to device

	@param iodevice device to write newline to.
*/
#define NEWLINE(iodevice) iodevice.write("\n")

using namespace JB;

namespace ImageImp {
	AlbumCreator::AlbumCreator() {
	}

	void AlbumCreator::createAlbum() {
		if (not album().isValid())
			throw new NoAlbumSelected;

		LOG("Creating album " << album().id());
		LOG("Collecting data for album");
		collectAlbumDate();

		if (not albumHasData())
			throw new AlbumIsEmpty;

		LOG("Creating export files for album");
		exportAlbum();
		sendToPhotoshop();
	}

	void AlbumCreator::collectAlbumDate() {
		collectPicturesAndPeople();
		collectExplicitPictureDefaults();
	}

	void AlbumCreator::exportAlbum() {
		writeAlbumSettings();
		writeAlbumPersonPictureList();
		writeExportHelperFile();
		sendToPhotoshop();
	}

	void AlbumCreator::writeExportHelperFile() const {
		QStringList files;
		files << albumExportSettingsPath() << albumExportDataPath() << destinationAlbumFile();

		QString data = files.join("\n");

		QFile export_helper_file(albumExportHelperPath());
		export_helper_file.open(QIODevice::Truncate | QIODevice::WriteOnly);
		export_helper_file.write(TO_C_STR(data));
		export_helper_file.close();
	}

	void AlbumCreator::writeAlbumSettings() const {
		LOG("Writing album export settings file");
		Album album(this->album());

		QFile album_settings_file(albumExportSettingsPath());
		album_settings_file.open(QIODevice::Truncate | QIODevice::WriteOnly);

		int width = ExportSettings::width();
		int height = ExportSettings::height();
		int picture_count = pictures_and_people.count();
		Matrix::fixDimentions(width, height, picture_count);

		album_settings_file.write(TO_C_STR(width));
		NEWLINE(album_settings_file);
		album_settings_file.write(TO_C_STR(height));
		NEWLINE(album_settings_file);

		QString template_path;

		template_path = album.pictureTemplate().path();
		if (not QFileInfo(template_path).exists())
			template_path = Template::defaultPictureTemplate();
		album_settings_file.write(TO_C_STR(template_path));
		NEWLINE(album_settings_file);

		template_path = album.albumTemplate().path();
		if (not QFileInfo(template_path).exists())
			template_path = Template::defaultPictureTemplate();
		album_settings_file.write(TO_C_STR(template_path));
		NEWLINE(album_settings_file);
		album_settings_file.close();
	}

	void AlbumCreator::collectPicturesAndPeople() {
		LOG("Collecting pictures and people names")
		QSqlRecord record;
		QString person;

		pictures_and_people.clear();

		QSqlQuery query("select people.id, people.name, pictures.path from people join ordered_people on people.id == item_id join pictures on pictures.person_id = people.id where pictures.album_id == :album_id and people.id not in (select item_id from excluded_people) group by people.id order by position");

		query.bindValue(":album_id", album().id());
		query.exec();

		while (query.next()) {
			record = query.record();
			person =record.value("name").toString();
			ordered_people << person;
			pictures_and_people.insert(person, record.value("path").toString());
		}

		LOG("Found " << pictures_and_people.count() << " pictures for the album");
	}

	void AlbumCreator::writeAlbumPersonPictureList() const {
		LOG("Writing album data export file")
		QFile data_file(albumExportDataPath());
		data_file.open(QIODevice::Truncate | QIODevice::WriteOnly);
		QStringList data_item;

		foreach (QString person, ordered_people) {
			data_item.clear();
			data_item << person << "\n" << pictures_and_people[person] << "\n";
			data_file.write(TO_C_STR(data_item.join("")));
		}

		data_file.close();
	}

	void AlbumCreator::collectExplicitPictureDefaults() {
		LOG("Finding explicit person picture defaults");
		QSqlRecord record;

		QSqlQuery query("select pictures.path, people.name from pictures join people on pictures.person_id = people.id join default_pictures on default_pictures.picture_id = pictures.id  where album_id = :album_id;");
		query.bindValue(":album_id", album().id());
		query.exec();
		while (query.next()) {
			record = query.record();
			pictures_and_people.insert(record.value("name").toString(), record.value("path").toString());
		}
	}

	void AlbumCreator::sendToPhotoshop() const {
		LOG("Sending data to photoshop");
		#ifdef Q_OS_WIN32
			QString program = "cscript";
			QStringList arguments;
			arguments << albumImportScriptPath();

			QProcess *export_process = new QProcess();
			export_process->start(program, arguments);
		#else
			throw new UnsupportedPlatform;
		#endif
	}

	QString AlbumCreator::albumExportDataPath() const {
		return Path::join(System::exportPath(), QString("album_%1_data.txt").arg(album().id()));
	}

	QString ImageImp::AlbumCreator::albumExportSettingsPath() const {
		return Path::join(System::exportPath(), QString("album_%1_settings.txt").arg(album().id()));
	}

	QString ImageImp::AlbumCreator::albumExportHelperPath() const {
		return Path::join(Path::userTempPath(), "imageimp_create_album.cfg");
	}

	QString ImageImp::AlbumCreator::albumImportScriptPath() const {
		return Path::join(System::scriptsPath(), "import.vbs");
	}

	QString AlbumCreator::destinationAlbumFile() const {
		QString base_album_psd_path = Path::join(System::exportPath(), QString("album_%1_export_%2.psd").arg(album().id()));

		QString album_psd_path;
		int i=1;

		do {
			album_psd_path = base_album_psd_path.arg(i);
			i++;
		} while (QFileInfo(album_psd_path).exists());

		return album_psd_path;
	}

}
