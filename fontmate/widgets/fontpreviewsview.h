#pragma once

/** QT headers **/
#include <QWidget>
#include <QScrollArea>

/** Application headers **/
#include <widgets/fontpreviewstack.h>
#include "ui_fontpreviewsview.h"

namespace FontMate {

	/**
		Container for font preview widgets.
	*/

	class FontPreviewsView : public QWidget, public Ui::FontPreviewsView {
		Q_OBJECT
		public:
			FontPreviewsView(QWidget *parent = 0);

			/**
				Make the temporary view into a selected view.
			*/
			void fixateTemporaryView();

			FontEditorPreview* lastFocusedPreviewEditor() const {
				return m_last_focused_preview;
			}

			FontPreviewStack* focusedPreview() const;

			void removeAllFontPreviews();

			void setDefaultPreviewText(FontEditorPreview *preview_widget);

			bool empty() const { return font_previews.empty(); }

			QList<FontPreviewStack*> previews() const { return font_previews; }

		signals:
			void fontPreviewAdded();
			void previewRemoveRequested(int index);
			void temporaryPreviewRemoved();
			void lastFontPreviewRemoved();
			void previewContentChanged();

			void previewContentChanged(QWidget*);

			/** Signals forwarded from individual editors **/
			void selectionBold(bool isBold);
			void selectionUnderlined(bool isUnderlined);
			void selectionItalic(bool isItalic);
			void selectionSize(qreal size);
			void selectionSize(const QString &size);
			void formattingChanged();

		public slots:

			/**
				Remove the temporary preview.
			*/
			void removeTemporaryView();

			void updateLastFocusedPreview(QWidget *old, QWidget *now);

			/**
				Add font to font preview container.
			*/
			void add(const QString &family);

			void loadInTemporaryView(const QString &family);

			/**
				Remove font from font preview container.

				@param id font index in preview list (not the font ID).
			*/
			void remove(int id);

			void updateAllPreviews(QWidget *source);

			/**
				Remove all previews
			*/
			void clear();

			void setDefaultFontSize(const QString& font_size) { setDefaultFontSize(font_size.toInt()); }
			void setDefaultFontSize(int font_size) { m_default_font_size = font_size; }

		private slots:
			/**

			*/
			void notifyPreviewRemoveRequest(QWidget *widget);

		private:
			FontEditorPreview* m_last_focused_preview;
			QList<FontPreviewStack*> font_previews;
			FontPreviewStack *temporary_preview;
			QVBoxLayout *previews_layout;
			int m_default_font_size;

		private:
			int defaultFontSize() const { return m_default_font_size; }
	};
}
