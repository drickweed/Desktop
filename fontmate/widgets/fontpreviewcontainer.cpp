#include "fontpreviewcontainer.h"

#include <QResizeEvent>
#include <QHBoxLayout>

namespace FontMate {

	FontPreviewContainer::FontPreviewContainer(QWidget *parent):
		QWidget(parent) {
		preview = new FontEditorPreview(this);
		title_bar = new FontPreviewBar(this);

		QHBoxLayout *layout = new QHBoxLayout;
		layout->addWidget(preview);
		layout->addWidget(title_bar);
		setLayout(layout);

		layout->setSpacing(1);
		layout->setContentsMargins(0,0,0,0);
	}


	FontPreviewContainer::~FontPreviewContainer() {
	}

}

