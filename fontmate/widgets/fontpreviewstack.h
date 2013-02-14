#pragma once

/** QT headers **/
#include <QWidget>
#include <QFont>

/** JB headers **/
#include <jb/widgets/label.h>

/** Application headers **/
#include "fontpreviewcontainer.h"

using namespace JB;

namespace FontMate {

	/**
		Stack comprised of a text editor and a font label. The font description label is visible over the font preview widget.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FontPreviewStack : public QWidget {
		Q_OBJECT
		public:
			FontPreviewStack(QWidget *parent = 0);

			~FontPreviewStack();

			FontPreviewContainer *preview;

			FontEditorPreview *editor() const { return preview->preview; }

			virtual QSize sizeHint () const;

			QString family() const { return editor()->fontFamily(); }

			bool syncContent() const { return m_sync_content; }

			QTextDocument* document() const { return editor()->document(); }

		public slots:
			void setFont(QFont font);
			void setFont(QString font);
			void updateLayout();

			void setSyncContent(bool state) { m_sync_content = state; }

		private slots:
			void requestPreviewClose();
			void editorContentsChangedEvent();

		signals:
			void previewCloseRequested(QWidget *widget);
			void previewContentChanged(QWidget *widget);

		protected:
			virtual void resizeEvent(QResizeEvent *event);

			/**
				@internal Hack to force font description label to appear in the correct place when creating a preview widget. If the widgets is resized, the label moves to the correct place without this hack, but is initially at the top left corner, half hidden.

			*/
			virtual void paintEvent(QPaintEvent *event);

		private:
			Label *label;

			bool m_sync_content;

			void repositionLabel();
	};

}
