#include "tagmanager.h"

#include <QVBoxLayout>

namespace FontMate {

	TagManager::TagManager(Document *document, QWidget *parent) :
		QDialog(parent),
		m_document(document) {
		setupUi(this);

		tagsView = new RelationalView(this);
		qobject_cast<QVBoxLayout*>(layout())->insertWidget(1, tagsView);
		tagsView->setModel(document->tags);

		connect(document->tags, SIGNAL(newItemAppended(QModelIndex)), tagsView, SLOT(startEditor(QModelIndex)), Qt::QueuedConnection);

		connect(addTagButton, SIGNAL(clicked()), document->tags, SLOT(appendNew()));
		connect(removeTagButton, SIGNAL(clicked()), SLOT(removeSelectedTag()));
	}

	void TagManager::removeSelectedTag() {
		QModelIndex index = tagsView->selectedIndex();
		if (not index.isValid())
			return;

		document()->tags->remove(index);
	}

	void TagManager::changeEvent(QEvent *e)
	{
		QDialog::changeEvent(e);
		switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
		}
	}
}
