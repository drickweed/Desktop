#include "tagselector.h"

TagSelector::TagSelector(Tags *tags, QWidget *parent) :
	QWidget(parent){

	Ui::TagSelector::setupUi(this);

	this->tags = tags;
	tagsProxy = new ItemTagsProxy(tags);

	setupUi();
}

void TagSelector::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

void TagSelector::setupUi() {
	setWindowFlags(Qt::Popup);

	tagsView->setModel(tagsProxy);
}
