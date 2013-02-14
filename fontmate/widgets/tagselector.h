#pragma once

#include "ui_tagselector.h"

/** JB headers **/
#include <jb/models/tags.h>
#include <jb/models/itemtagsproxy.h>

using namespace JB;

class TagSelector : public QWidget, private Ui::TagSelector {
	Q_OBJECT
	public:
		TagSelector(Tags *tags, QWidget *parent = 0);

	protected:
		void changeEvent(QEvent *e);

	private:

		void setupUi();

		ItemTagsProxy *tagsProxy;
		Tags *tags;
};
