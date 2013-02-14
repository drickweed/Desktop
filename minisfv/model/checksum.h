#pragma once

/** QT headers **/
#include <QString>
#include <QFile>
#include <QMutex>

namespace MiniSFV {

	/**
		Base checksum generator class.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class ChecksumGenerator : public QFile {
		Q_OBJECT

		public:
			/**
				Constructor.
			*/
			ChecksumGenerator(QString path);

			/** Generate checksum

				@param total_bytes_read total bytes read
				@return checksum
			*/
			virtual unsigned long generate(unsigned long &total_bytes_read) = 0;

		signals:
			void bytesRead(int bytes); /// Bytes read so far
			void checksumResult(unsigned long checksum); /// Result
			void fileSize(int bytes); /// File size
			void aborted(); /// Aborted

		public slots:
			void abort(); /// Abort generating checksum

		protected:
			/** Specify if checksum generator should continue

				@param value Continue generating checksum
			 */
			void setContinueProcessing(bool value);

			/** Check if checksum generator should continue

			@return true if allowed to continue generating checksum
			 */
			bool getContinueProcessing();

		private:
			QMutex processing_lock;
			bool continue_processing; ///> Continue checksum calculation.
	};
}
