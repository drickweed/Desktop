#include "checksum.h"

namespace MiniSFV {
	ChecksumGenerator::ChecksumGenerator(QString path):
			QFile(path),
			continue_processing(true) {
	}

	bool ChecksumGenerator::getContinueProcessing(){
		processing_lock.lock();
		bool value = continue_processing;
		processing_lock.unlock();
		return value;
	}

	void ChecksumGenerator::setContinueProcessing(bool value) {
		processing_lock.lock();
		continue_processing = value;
		processing_lock.unlock();
	}

	void ChecksumGenerator::abort() {
		setContinueProcessing(false);
	}
}
