/** QT headers **/
#include <QtDebug>

/** Application headers **/
#include "crc32checksum.h"

/** JB headers **/
#include <jb/std/exceptions.h>

using namespace JB;

namespace MiniSFV {

	CRC32Checksum::CRC32Checksum(QString path):
		ChecksumGenerator(path) {
	}

	unsigned long CRC32Checksum::generate(unsigned long &total_bytes_read) {

		char	buffer[BUFFERSIZE]; /// Data read from file
		int		data_size=0; /// Byte count in last read action
		char	*p;
		uint32_t crc32 = ~0; /// Final crc32 value

		total_bytes_read = 0; /// Total bytes read

		// We were just called. Assume we arn't immediatly aborted.
		setContinueProcessing(true);

		emit fileSize(size());

		try {
			bool open_result = open(QIODevice::ReadOnly);

			if (not open_result)
				throw FileOpenError();
			// Check open failure

			while (not atEnd()) {

				//Check for abort request:
				if (not getContinueProcessing()) {
 					throw ProcessAborted();
				}

				// Read data:
				data_size = read(buffer, sizeof(buffer));

				// Error reading data:
				if (data_size == -1) {
					throw FileReadError();
				}

				// Update total bytes read:
				total_bytes_read += data_size;

				// Signal progress:
				emit bytesRead(total_bytes_read);

				// Update CRC value with additional data:
				for (p = buffer; data_size--; ++p) {
					crc32 = (crc32 >> 8) ^ crctable[(crc32 ^ *p) & 0xff];
				}

			}

			crc32 = ~crc32;

			emit checksumResult(crc32);

		} catch (ProcessAborted &e) {
			emit aborted();
		} catch (FileOpenError &e) {
			emit aborted();
		} catch (FileReadError &e) {
			emit aborted();
		}

		// Close file no matter what.
		close();
		return crc32;
	}
}
