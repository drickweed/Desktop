/** QT headers **/
#include <QDebug>
#include <QColor>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QChar>

/** Application headers **/
#include "filechecksummodel.h"

/** JB headers **/
#include <jb/qt4/pathutils.h>

using namespace JB;
using namespace JB::Path;

namespace MiniSFV {

	/**
		Check if file is a checksum file database.

		@param path path to checksum database file.
		@return True if file is a checksum database file.
	*/
	bool isChecksumFile(QString path) {
		return (QFileInfo(path).isFile() and (QString::compare(path.right(4).toLower(), ".sfv") == 0));
	}

	FileChecksumModel::FileChecksumModel(QObject *parent):
		QAbstractTableModel(parent),
		checksum_filename("checksums"),
		checksum_filepath(QDir::toNativeSeparators(QDir::homePath())) {

// 		background_brushes.append(QBrush(QColor(255,255,255))); // Default
// 		background_brushes.append(QBrush(QColor(255,150,150))); // Invalid
// 		background_brushes.append(QBrush(QColor(127, 255, 212))); // Missing file
//
// 		foreground_brushes.append(QBrush(QColor(0,0,0))); // Unknown
// 		foreground_brushes.append(QBrush(QColor(50,150,10))); // Valid

		valid = QImage(":/images/valid.png");
		invalid = QImage(":/images/invalid.png");
		unknown = QImage(":/images/unknown.png");
		missing = QImage(":/images/missing.png");
	}

	FileChecksumModel::~FileChecksumModel() {
	}

	Qt::DropActions FileChecksumModel::supportedDropActions() const {
		return Qt::CopyAction | Qt::MoveAction;
	}

	QStringList FileChecksumModel::mimeTypes() const {
		QStringList types;
		types << "text/uri-list";
		return types;
	}

	void FileChecksumModel::loadFilesFromUrls(QList<QUrl> urls) {

		if (urls.empty())
			return;

		int url_count = urls.size();
		int records_inserted = 0;
		int first_inserted_row = rowCount();

		// Only adds real files from the list. no recursion. recursion should have been done elsewhere if needed.
		for(int i=0; i<url_count; i++) {
			QString filename = urls[i].toLocalFile();

			if (filename.isEmpty())
				continue;

			if (isChecksumFile(filename))
				continue;

			add(filename);
			records_inserted++;
		}

		if (records_inserted > 0) {
			beginInsertRows(QModelIndex(), first_inserted_row, first_inserted_row + records_inserted - 1);
			endInsertRows();
		}
	}

	bool FileChecksumModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int /* row */, int /* column */, const QModelIndex& /* parent */) {

		if (action == Qt::IgnoreAction)
			return true;

		if (not data->hasUrls())
			return false;

		QList<QUrl> urls = data->urls();

		//FIXME code to remove last empty url. bug in qt ?
		if (urls.last().isEmpty())
			urls.removeLast();

		QStringList url_names = toLocalFiles(urls);
		loadUrls(url_names);

		return true;
	}

	QVariant FileChecksumModel::headerData(int section, Qt::Orientation orientation, int role) const {
		if (role == Qt::DisplayRole)
			if (orientation == Qt::Horizontal) {
				switch (section) {
					case ChecksumModel::FILENAME: return QVariant(QString(tr("File name")));
				}
			}
		return QVariant();
	}

	int FileChecksumModel::rowCount(const QModelIndex& /* parent */) const {
		return records.size();
	}

	int FileChecksumModel::columnCount(const QModelIndex& /* parent */) const {
		return ChecksumModel::COLUMN_COUNT;
	}

	QVariant FileChecksumModel::data(const QModelIndex & index, int role) const {

		const FileChecksumRecord record = records[index.row()];
		QString path = record.path();

		switch (role) {
			case Qt::DisplayRole:
				switch (index.column()) {
					case ChecksumModel::FILENAME:
						return QVariant(minimalPathName(path));
						break;
					default: break;
				}
				break;

			case Qt::ToolTipRole:
				if (not record.hasCalculatedChecksum()) {
					return QVariant(QString("Checksum: Not checked\nFile: ") + path);
				} else {
					return QVariant(QString("Checksum: ") + checksumStr(record.calculatedChecksum()) + "\nFile: " + path);
				}
				break;

			case Qt::BackgroundRole:
				if (not record.fileExists())
					return QVariant(QBrush(QColor(127, 255, 212)));
				else if (record.isInvalid())
					return QVariant(QBrush(QColor(255,150,150)));
				return QVariant(QBrush(QColor(255,255,255)));
				break;

			case Qt::ForegroundRole:
				if (record.isValid())
					return QVariant(QBrush(QColor(50,150,10)));
				return QVariant(QBrush(QColor(0,0,0)));
				break;

			case Qt::DecorationRole:
				if (not (record.fileExists()))
					return QVariant(missing);
				else if (record.isUnknown())
					return QVariant(unknown);
				else if (record.isInvalid())
					return QVariant(invalid);
				else if (record.isValid())
					return QVariant(valid);
				break;
		}

		return QVariant();
	}

	int FileChecksumModel::add(QString path) {
		FileChecksumRecord record(path);
		records.append(record);

		int new_record_index = rowCount()-1;
		filename_index[record.path()] = new_record_index;
		return new_record_index;
	}

	void FileChecksumModel::remove(unsigned long index) {
		beginRemoveRows(QModelIndex(), index, index);
		filename_index.remove(records[index].path());
		records.removeAt(index);
		endRemoveRows();
		emit recordCountChanged(rowCount());
	}

	Qt::ItemFlags FileChecksumModel::flags(const QModelIndex &index) const {
		if (index.isValid()) {
			return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}

		return Qt::ItemIsDropEnabled;
	}

	void FileChecksumModel::setChecksum(int index, unsigned long checksum) {
		records[index].setCalculatedChecksum(checksum);
		// Announce change
		emit calculatedChecksumSet(index);
	}

	bool FileChecksumModel::loadChecksumsFromFile(QString path, bool addRecords) {
		int index; // Index of record in database
		bool ok; // Used in toInt conversion of checksum
		int rows_added=0; // Number of rows added to model
		int first_new_row = rowCount(QModelIndex());

		QFile checksum_file(path);

		// Base path for urls in the checksum file is the checksum files' current location
		QString files_basepath = QFileInfo(path).path();

		if (not checksum_file.open(QFile::ReadOnly))
			return false;

		QTextStream data(&checksum_file);
		while (not data.atEnd()) {
			QString line = data.readLine();

			// Fix data line for possible problems:
			// 1) Trailing and starting whitespace
			line = line.trimmed();

			// If we have something to work with
			if (not line.isEmpty()) {

				// Ignore comments (lines starting with ;)
				// NOTE not going to implement non-standard data saved in comments by other applications.
				if (QString::compare(line.at(0), ";") == 0)
					continue;

				// Ignore invalid lines
				// NOTE invalid line: less than 10 characters (8 for checksum + minimum 1 file name + minumum 1 space)
				if (line.size() < 10)
					continue;

				// Parse checksum segment
				QString checksum_string = line.right(8);
				uint checksum = checksum_string.toUInt(&ok, 16);
				// Parse filename segment
				QString filename = line.left(line.size()-9).trimmed();
				filename = join(files_basepath, filename);

				// Check if file exists in model
				try {
// 					qDebug() << "Trying existing" << filename;
					records[recordIndex(filename)].setStoredChecksum(checksum);

				} catch (RecordNotFound *e) {
					if (addRecords) {
// 						qDebug() << "Adding" << filename;
						index = add(filename);
						records[index].setStoredChecksum(checksum);
						rows_added++;
					}
				}

			}
		}

		updateBasePath();

		if (rows_added > 0) {
			beginInsertRows(QModelIndex(), first_new_row, first_new_row + rows_added - 1);
			endInsertRows();
		}

		checksum_file.close();
		return true;
	}

	const FileChecksumRecord& FileChecksumModel::getRecord(QString path) const {
		if (filename_index.contains(path))
			return records[filename_index[path]];

		throw new RecordNotFound;
	}

	void FileChecksumModel::setChecksumDatabaseFile(QString path) {
		checksum_filepath = QFileInfo(path).path();
		checksum_filename = QFileInfo(path).baseName();
	}

	void FileChecksumModel::saveChecksums() {
		QFile checksum_file(checksumFileName());
		bool result = checksum_file.open(QIODevice::WriteOnly|QIODevice::Truncate);

		if (not result) {
			emit errorWritingChecksumFile();
			return;
		}

		QTextStream output(&checksum_file);
		int count = records.size();

		for(int i=0; i < count; i++) {
			const FileChecksumRecord record = records[i];
			if (record.hasCalculatedChecksum()) {
				QString checksum_str = checksumStr(record.calculatedChecksum());
				output << QString("%1 %2\r\n").arg(getRelativePath(record.path())).arg(checksum_str);
			}
		}
		checksum_file.close();
		emit savedChecksumFile();
	}

	QString FileChecksumModel::checksumFileName() const {
		return join(checksum_filepath, checksum_filename) + ".sfv";
	}

	QString FileChecksumModel::checksumStr(unsigned long checksum) const {
		return QString("%1").arg(checksum, 8, 16, QChar('0')).toUpper();
	}

	//TODO filter framework for QList
	uint FileChecksumModel::validCount() const {
		uint count = 0,
			 size = records.size();
		for(uint i=0; i<size; i++ )
			if (records[i].isValid())
				count++;
		return count;
	}

	uint FileChecksumModel::invalidCount() const {
		uint count = 0,
			 size = records.size();
		for(uint i=0; i<size; i++ )
			if (records[i].isInvalid())
				count++;
		return count;
	}

	uint FileChecksumModel::missingCount() const	{
		uint count = 0,
			 size = records.size();
		for(uint i=0; i<size; i++ )
			if (not records[i].fileExists())
				count++;
		return count;
	}

	uint FileChecksumModel::uncheckedCount() const {
		uint count = 0,
			 size = records.size();
		for(uint i=0; i<size; i++ )
			if (records[i].fileExists() and records[i].isUnknown())
				count++;
		return count;
	}

	void FileChecksumModel::updateBasePath() {
		base_path = commonBasePath(files());
	}

	QStringList FileChecksumModel::files() const {
		QStringList files;
		int count = records.size();
		for(int i=0; i<count; i++) {
			files.append(records[i].path());
		}
		return files;
	}

	void FileChecksumModel::loadUrls(QStringList &urls) {
		int url_count = urls.size();

		// Only one argument, and it is a checksum file
		if ((url_count == 1) and (isChecksumFile(urls[0]))) {
			bool result = loadChecksumsFromFile(urls[0], true);
			if (result) { // Succesfully loaded chekcum file
				setChecksumDatabaseFile(urls[0]);
				emit loadedChecksumFile();
// 				emit filesAdded();
				emit recordCountChanged(rowCount());
			}

		// Any other arguments, treat as urls and folders.
		} else if (url_count > 0) {

			QStringList real_files;
			for (int i=0; i<url_count; i++) {
				if (QFileInfo(urls[i]).isFile())
					real_files.append(urls[i]);
				else if (QFileInfo(urls[i]).isDir()) {
					QStringList contents = getFilesInDir(urls[i]);
					for(int j=0; j<contents.size(); j++)
						real_files.append(contents[j]);
				}
			}

			loadFilesFromUrls(fromLocalFiles(real_files));

			// Select default name and path for checksum file
			setDefaultChecksumResultFileName(urls[0]);

			updateBasePath();

			emit filesAdded();
			emit recordCountChanged(rowCount());
		}
	}

	QString FileChecksumModel::minimalPathName(QString path) const {
		return path.right(path.size() - base_path.size());
	}

	QString FileChecksumModel::getRelativePath(QString path) const {
		return QDir(checksum_filepath).relativeFilePath(path);
	}

	const FileChecksumRecord& FileChecksumModel::getRecord(int index) const {
		return records[index];
	}

	void FileChecksumModel::unsetCalculatedChecksum(int index) {
		records[index].unsetCalculatedChecksum();
		emit calculatedChecksumSet(index);
	}

	void FileChecksumModel::setDefaultChecksumResultFileName(QString filename) {
		checksum_filepath = QFileInfo(filename).path();

		if (QFileInfo(filename).isDir()) {
			checksum_filename = QFileInfo(QFileInfo(QFileInfo(filename).absoluteFilePath()).path()).baseName();
		} else
			checksum_filename = QFileInfo(filename).baseName();
	}

	int FileChecksumModel::recordIndex(QString path) const {
		if (filename_index.contains(path))
			return filename_index[path];

		throw new RecordNotFound;
	}

	QFile::FileError FileChecksumModel::checksumFileCanBeWritten() {
		QFile checksum_file(checksumFileName());
		checksum_file.open(QIODevice::WriteOnly|QIODevice::Truncate);
		QFile::FileError error = checksum_file.error();
		checksum_file.close();
		return error;
	}

}
