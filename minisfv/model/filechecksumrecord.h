#pragma once

/** QT headers **/
#include <QString>

/** STL headers **/
#include <exception>

using namespace std;

namespace MiniSFV {

	class NonExistantChecksumRequested : public exception {
		virtual const char* what() const throw() {
			return "Checksum requested but none exists.";
		}
	};

	/** File checksum record.

		Includes the full path to the file, the calculated and stored checksums if they exist.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FileChecksumRecord {

		public:
			/**
				Default constructor.
			*/
			FileChecksumRecord();

			/**
				Convenience constructor.

				@param path Full path to file.
				@param stored_checksum Stored checksum.
			*/
			FileChecksumRecord(QString path, unsigned long stored_checksum);

			/**
				Convenience constructor.

				@param path Full path to file
			 */
			FileChecksumRecord(QString path);

			/**
				Copy constructor.
			*/
			FileChecksumRecord(const FileChecksumRecord &other);

			/**
				Destructor
			*/
			~FileChecksumRecord();

			/**
				File name (last element in full path).

				@return file name.
			*/
			QString fileName() const;

			unsigned long calculatedChecksum() const;
			unsigned long storedChecksum() const;

			void setCalculatedChecksum(unsigned long checksum);

			void setStoredChecksum(unsigned long checksum);

			void unsetCalculatedChecksum();

			/**
				@return True if path is not an empty string.
			*/
			bool valid() const { return not path().isEmpty(); }

			/**
				Record is valid.

				@return True if file exists and expected checksum matches stored checksum if they both exist.
			*/
			bool isValid() const;

			/**
				Record is invalid.

				@returns True if file exists, but calculated checksum differs from stored checksum.
			*/
			bool isInvalid() const;

			/**
				Record state unknown.

				@returns True if file exists, and either calculated or stored checksum is missing.
			*/
			bool isUnknown() const;

			bool isMissing() const { return not fileExists(); }

			QString path() const { return m_path; } /// Path to file

			void setPath(QString path) {
				m_path = path;
				updateFileExistanceInfo();
			}

			/**
				Check if file exists and update m_file_exists property.
			*/
			void updateFileExistanceInfo();

			bool hasCalculatedChecksum() const { return calculated_checksum != NULL; }
			bool hasStoredChecksum() const { return stored_checksum != NULL; }

			bool fileExists() const { return m_file_exists; }

			/**
				@return True if both checksums (stored and calculated) are known.
			*/
			bool allChecksumsKnown() const;

		private:
			unsigned long *calculated_checksum; ///> Calculated checksum.
			unsigned long *stored_checksum; ///> Stored checksum.
			bool m_file_exists; ///> File exists.
			QString m_path; ///> Full file path.

	};
}
