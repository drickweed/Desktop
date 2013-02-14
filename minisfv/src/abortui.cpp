#include "abortui.h"

/** JB headers **/
#include <jb/std/memory.h>

namespace MiniSFV {

	AbortUI::AbortUI(QWidget *parent):
		QWidget(parent) {

		abort_btn = new QPushButton(tr("Stop"), this);
		connect(abort_btn, SIGNAL(clicked()), SIGNAL(abort()));

		skip_btn = new QPushButton(tr("Skip"), this);
		connect(skip_btn, SIGNAL(clicked()), SIGNAL(skip()));

		layout = new QHBoxLayout();
		layout->addWidget(skip_btn);
		layout->addWidget(abort_btn);
		setLayout(layout);
	}

	AbortUI::~AbortUI() {
		DELETE(layout);
		DELETE(abort_btn);
		DELETE(skip_btn);
	}
}
