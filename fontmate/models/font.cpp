#include "font.h"

namespace FontMate {

	Font* Font::fromQFont(QFont qt_font) {
		Font *font = new Font;
		font->setFamily(qt_font.family());
		font->setWeight(qt_font.weight());

		return font;
	}

}
