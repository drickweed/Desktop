
// Application includes
#include "filechecksumrecord.h"

/** JB headers **/
#include <jb/std/memory.h>

/** QT headers **/
#include <QFile>
#include <QFileInfo>

namespace MiniSFV {

	FileChecksumRecord::FileChecksumRecord():
		calculated_checksum(NULL),
		stored_checksum(NULL),
		m_file_exists(false) {
	}

	FileChecksumRecord::FileChecksumRecord(QString path, unsigned long checksum):
		calculated_checksum(NULL) {

		setPath(path);
		setStoredChecksum(checksum);
	}

	FileChecksumRecord::FileChecksumRecord(QString path):
		calculated_checksum(NULL),
		stored_checksum(NULL) {

		setPath(path);
	}

	FileChecksumRecord::~FileChecksumRecord(){
		DELETE(calculated_checksum);
		DELETE(stored_checksum);
	}

	bool FileChecksumRecord::isValid() const {
		// Both checksums are known and are the same
		return (fileExists() and allChecksumsKnown() and (*calculated_checksum == *stored_checksum));
	}

	bool FileChecksumRecord::isInvalid() const {
    	// Both checksums are known and are NOT the same
		return (fileExists() and allChecksumsKnown() and (*calculated_checksum != *stored_checksum));
	}

	bool FileChecksumRecord::isUnknown() const {
		return (fileExists() and not allChecksumsKnown());
	}

	void FileChecksumRecord::updateFileExistanceInfo() {
		m_file_exists = QFile::exists(path());
	}

	void FileChecksumRecord::setCalculatedChecksum(unsigned long checksum) {
		if (not calculated_checksum) {
			calculated_checksum = new (unsigned long);
		}
		*calculated_checksum = checksum;
	}

	void FileChecksumRecord::setStoredChecksum(unsigned long checksum) {
		if (not stored_checksum) {
			stored_checksum = new (unsigned long);
		}
		*stored_checksum = checksum;
	}

	QString FileChecksumRecord::fileName() const {
		return QFileInfo(path()).fileName();
	}


	bool FileChecksumRecord::allChecksumsKnown() const {
		return (calculated_checksum and stored_checksum);
	}

	unsigned long FileChecksumRecord::calculatedChecksum() const {
		if (calculated_checksum)
			return *calculated_checksum;
		else
			throw new NonExistantChecksumRequested;
	}

	unsigned long FileChecksumRecord::storedChecksum() const {
		if (stored_checksum)
			return *stored_checksum;
		else
			throw new NonExistantChecksumRequested;
	}

	void FileChecksumRecord::unsetCalculatedChecksum() {
		DELETE(calculated_checksum);
	}

	FileChecksumRecord::FileChecksumRecord(const FileChecksumRecord &other):
		calculated_checksum(NULL),
		stored_checksum(NULL) {

		setPath(other.path());
		if (other.hasCalculatedChecksum())
			setCalculatedChecksum(other.calculatedChecksum());
		if (other.hasStoredChecksum())
			setStoredChecksum(other.storedChecksum());
	}

}
