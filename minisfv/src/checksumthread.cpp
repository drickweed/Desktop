// Application includes
#include "checksumthread.h"
#include "memory"

namespace MiniSFV {

	ChecksumThread::ChecksumThread(ChecksumGenerator *checksum, QObject *parent):
		QThread(parent),
		checksum(checksum) {
	}

	void ChecksumThread::run() {
		unsigned long bytes_processed;
		checksum->generate(bytes_processed);
	}
}
