#include "validators.h"

namespace Pdf {
	namespace Validators {
		bool isDictionary(QByteArray data) {
			return data.startsWith("<<") && data.endsWith(">>");
		}
	}
}
