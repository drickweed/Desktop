#pragma once

#include <QWidget>
#include <QPushButton>

#include "fonteditorpreview.h"
#include "fontpreviewbar.h"

namespace FontMate {

	/**
		Combination of the font preview and it's management buttons :
		- Close preview.
		- Enable/Disable sync mode.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FontPreviewContainer : public QWidget {
		Q_OBJECT

		public:
			FontPreviewContainer(QWidget *parent=0);

			~FontPreviewContainer();

			FontEditorPreview *preview;
			FontPreviewBar *title_bar;

			QPushButton* closeButton() const { return title_bar->closeButton; }


	};

}
