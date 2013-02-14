#pragma once

/** QT headers **/
#include <QTextEdit>
#include <QFont>
#include <QString>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QKeyEvent>

namespace FontMate {

	/**
		Extended text editor. Methods to directly control the contents and formatting.
	*/
	class FontEditorPreview : public QTextEdit {

		Q_OBJECT

		public:
			FontEditorPreview(QWidget* parent=0);
			~FontEditorPreview();

			QString fontFamily() const { return m_font_family; }

			void copyTextFrom(FontEditorPreview *other);

		public slots:
			void setFont(QFont font);
			void setFont(QString font);
			void setText(QString text);

			void setBoldFormat(bool enabled);
			void setUnderlineFormat(bool enabled);
			void setItalicFormat(bool enabled);
			void setSize(int size);
			void resizeAllText(int size);

		signals:
			void textModified(QString text);

			void selectionBold(bool isBold);
			void selectionUnderlined(bool isUnderlined);
			void selectionItalic(bool isItalic);
			void selectionSize(qreal size);
			void selectionSize(const QString &size);
			void formattingChanged();

			void textInserted();

		private:
			QString m_font_family;
			void updatePreviewFont();

			bool zoom_mode;

			QTextCharFormat cursorFormat() const;
			void setCursorFormat(QTextCharFormat format);

			int firstCharacterSize();

		private slots:
			void getSelectionFormat();
			void zoomIn();
			void zoomOut();
			void pasteHTML();

		protected:
			virtual void keyPressEvent(QKeyEvent *event);
			virtual void keyReleaseEvent(QKeyEvent *event);
			virtual void wheelEvent(QWheelEvent *event);
			virtual void contextMenuEvent(QContextMenuEvent *event);
			virtual void insertFromMimeData(const QMimeData *source);

	};

}
