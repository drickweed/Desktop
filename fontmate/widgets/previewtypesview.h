#pragma once

#include "ui_previewtypesview.h"

class PreviewTypesView : public QWidget, private Ui::PreviewTypesView {
		Q_OBJECT
	public:
		PreviewTypesView(QWidget *parent = 0);

	protected:
		void changeEvent(QEvent *e);

	private:

};

