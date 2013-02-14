#pragma once

/** QT headers **/
#include <QStringList>
#include <QWidget>

namespace FontMate {
	namespace Print  {
		void fontList(QStringList fonts, QWidget *parent=0, QString preview_text=QString(), int font_size=22);

	}

}
