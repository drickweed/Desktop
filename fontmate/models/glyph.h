#pragma once

/** QT headers **/
#include <QFont>
#include <QChar>

namespace FontMate {

	class Glyph {
		public:
			Glyph();

		private:
			QFont m_font;
			QChar m_character;
	};

}
