#include "fontpreviewtabs.h"

namespace FontMate {

	FontPreviewTabs::FontPreviewTabs(QWidget *parent) :
		QTabWidget(parent) {
		m_previews = new FontPreviewsView(this);
		m_scrachboard_view = new ScratchBoardView(this);
		m_glyphs_view = new GlyphsView(this);

		addTab(m_previews, tr("Comparison"));
		addTab(m_glyphs_view, tr("Glyphs"));
		addTab(m_scrachboard_view, tr("Scratch Board"));

		setFont(qApp->font());
	}
}
