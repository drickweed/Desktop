#pragma once

/** QT headers **/
#include <QList>
#include <QStringList>
#include <QUrl>
#include <QString>
#include <QAbstractTableModel>
#include <QMimeData>
#include <QBrush>
#include <QImage>
#include <QHash>
#include <QFile>

/** Application headers **/
#include "filechecksumrecord.h"

/** STL headers **/
#include <exception>

using namespace std;

namespace MiniSFV {

	namespace ChecksumModel {
		enum Columns {FILENAME, COLUMN_COUNT};
	}

	namespace ChecksumRecordState {
		enum Backgrounds {DEFAULT, INVALID, MISSINGFILE};
		enum Foregrounds {UNKNOWN, VALID};
	}

	class RecordNotFound : public exception {

	};

	/**
		File checksum list model.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FileChecksumModel : public QAbstractTableModel {
		Q_OBJECT

		public:
			/**
				Constructor.

				@param parent Parent QObject
			*/
			FileChecksumModel(QObject *parent=0);

			/**
				Destructor.
			*/
			virtual ~FileChecksumModel();

			/** QT Api **/
			virtual Qt::DropActions supportedDropActions() const;
			virtual Qt::ItemFlags flags(const QModelIndex &index) const;
			virtual QStringList mimeTypes() const;
			virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
			virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
			virtual int columnCount(const QModelIndex &parent=QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

			/**
				Set the filename containing the stored checksums.

				@param path path to file containing stored checksums.
			*/
			void setChecksumDatabaseFile(QString path);

			/**
				@return True if model is empty.
			*/
			bool empty() const { return records.empty(); }

			/**
				Return filename of checksum file.

				@return Name for checksum file
			*/
			QString checksumFileName() const;

			/**
				Set calculated checksum for record at given index.

				@param index record index
				@param checksum file checksum
			*/
			void setChecksum(int index, unsigned long checksum);

			/**
				Add checksum record with only file name.

				@param path Full path to file.
				@return index of added record.
			 */
			int add(QString path);

			/**
				Load checksums from file.

				@param path File name
				@param addRecords Add files to list. Otherwise only loads checksums for files alreaFileChecksumRecord&dy in list
				@return True on success

				@todo Checksum file parsing will be moved to checksum plugins when implemented.
			*/
			bool loadChecksumsFromFile(QString path, bool add_unexisting_files=false);

			// Get number of record for various types

			uint uncheckedCount() const;
			uint missingCount() const;
			uint invalidCount() const;
			uint validCount() const;

			/**
				Load files from url list.

				@param urls url list
			*/
			void loadFilesFromUrls(QList<QUrl> urls);

			/**
				Get list of paths.

				@return list of full file paths.
			*/
			QStringList files() const;

			/** Get record for file full path

				@param path full path of file to get record for
			 */
			const FileChecksumRecord& getRecord(QString path) const;

			const FileChecksumRecord& getRecord(int index) const;

			int recordIndex(QString path) const;

			/** Remove record at index

				@param index Index of record to remove
			*/
			void remove(unsigned long index);

			void unsetCalculatedChecksum(int index);

			QString basePath() const { return base_path; }

			QFile::FileError checksumFileCanBeWritten();

		public slots:
			/**
				Save checksums to file
			*/
			void saveChecksums();

			/**
				Load urls.

				Urls can be either files or directories.
				If a single file is given and it is a checksum file (.sfv) its contents will be loaded. If multiple files and folders are given, the files will be added, and the file contents of the folders will be added recursively.

				@param urls list of urls to load
			*/
			void loadUrls(QStringList &urls);

		signals:
			void errorWritingChecksumFile();
			void rowsAdded(QModelIndex start, QModelIndex end);
			void loadedChecksumFile();
    		void filesAdded();
    		void savedChecksumFile();

			void recordCountChanged(int count);

			void calculatedChecksumSet(int index);

		private:

			// Properties

			QImage valid, invalid, missing, unknown;

			QList<FileChecksumRecord> records; ///> File checksum records
			QHash<QString, int> filename_index; ///> (Filename -> Record number) index

			QString checksum_filename; ///> Checksum file base name
			QString checksum_filepath; ///> Checksum file base path
			QString base_path; ///> Common base path for files in model. will be removed from all files for display.

			// FIXME stopped using brush list because it started causing segfaults for unknown reason
			//QList<QBrush> background_brushes  /// Background brushes
			//			 ,foreground_brushes; /// Foreground brushes

			// Methods

			/**
				Checksum as zero padded string.

				@param checksum checksum
			*/
			QString checksumStr(unsigned long checksum) const;

			QString minimalPathName(QString path) const;

			/**
				Update common base path for all files in model.
			*/
			void updateBasePath();

			QString getRelativePath(QString path) const;

			/**
				Set a default checksum result file name and path from the given filename.
			*/
			void setDefaultChecksumResultFileName(QString filename);

	};
}
