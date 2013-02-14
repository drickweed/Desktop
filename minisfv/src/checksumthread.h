#pragma once

// QT includes
#include <QThread>

// Application includes
#include "checksum.h"

namespace MiniSFV {

	/** Checksum generation thread

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class ChecksumThread : public QThread {
		Q_OBJECT

		public:
			/** Constructor

				@param checksum Checksum generator
				@param parent parent
			*/
			ChecksumThread(ChecksumGenerator *checksum, QObject *parent=0);

		public slots:
			virtual void run(); /// Run thread

		private:
			ChecksumGenerator *checksum; /// Checksum generator
	};
}
