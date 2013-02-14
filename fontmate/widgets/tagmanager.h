#pragma once

#include "ui_tagmanager.h"

/** Application headers **/
#include <document/document.h>

/** JB headers **/
#include <jb/database/view.h>

using namespace JB;

namespace FontMate {

	class TagManager : public QDialog, private Ui::TagManager {
		Q_OBJECT
		public:
			TagManager(Document *document, QWidget *parent=0);

		protected:
			void changeEvent(QEvent *e);

			Document* document() const { return m_document; }

		private slots:
			void removeSelectedTag();

		private:
			Document *m_document;

			RelationalView *tagsView;
	};
}
