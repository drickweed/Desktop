#pragma once

/** QT headers **/
#include <QWidget>
#include <QTabWidget>

/** Application headers **/
#include <widgets/fontpreviewsview.h>
#include <widgets/scratchboard.h>
#include <widgets/glyphsview.h>

namespace FontMate {

	/**
		Container for various font preview methods.
		- Text comparison
		- Glyph comparison
		- Scratchboard
	*/
	class FontPreviewTabs : public QTabWidget {
		Q_OBJECT
		public:
			FontPreviewTabs(QWidget *parent=0);

		signals:

		public:
			FontPreviewsView *m_previews;
			ScratchBoardView *m_scrachboard_view;
			GlyphsView *m_glyphs_view;

	};
}
