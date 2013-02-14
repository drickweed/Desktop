#include "fontpreviewsview.h"

/** Application headers **/
#include "settings/settings.h"

namespace FontMate {

	FontPreviewsView::FontPreviewsView(QWidget *parent):
		QWidget(parent),
		Ui::FontPreviewsView(),
		m_last_focused_preview(NULL),
		temporary_preview(NULL),
		m_default_font_size(14) {
		Ui::FontPreviewsView::setupUi(this);

		previews_layout = qobject_cast<QVBoxLayout*>(previewsContainer->layout());

	}

	void FontPreviewsView::fixateTemporaryView() {
		temporary_preview = NULL;
	}

	void FontPreviewsView::removeTemporaryView() {
		if (temporary_preview) {
			// The temporary preview is always the last one
			// FIXME: does this make sense ?
			remove(font_previews.count()-1);
			temporary_preview = NULL;
			emit temporaryPreviewRemoved();
		}
	}

	void FontPreviewsView::loadInTemporaryView(const QString &family) {
		if (temporary_preview) {
			// qDebug() << "Loading font in quick preview" << family;
			temporary_preview->setFont(family);
		}
		else {
			add(family);
			temporary_preview = font_previews.last();
		}
	}

	void FontPreviewsView::clear(){
		removeTemporaryView();

		while (not font_previews.empty())
			remove(0);
	}

	void FontPreviewsView::updateLastFocusedPreview(QWidget* /* old */, QWidget *now) {
		FontEditorPreview* widget = qobject_cast<FontEditorPreview*>(now);
		if (widget)
			m_last_focused_preview = widget;
	}

	void FontPreviewsView::notifyPreviewRemoveRequest(QWidget *widget) {

		FontPreviewStack *preview = qobject_cast<FontPreviewStack*>(widget);

		if (not preview)
			return;

		if (preview == temporary_preview) {
			removeTemporaryView(); // Bypass unselecting font in model
			//fontTableView->clearSelection();
			return;
		}

		emit previewRemoveRequested(previews().indexOf(preview));
	}

	FontPreviewStack* FontPreviewsView::focusedPreview() const {
		if (m_last_focused_preview) {
			QObject *grandparent = m_last_focused_preview->parent()->parent();
			return qobject_cast<FontPreviewStack*>(grandparent);
		}

		return NULL;
	}

	void FontPreviewsView::updateAllPreviews(QWidget *source_widget) {
		FontPreviewStack *source = qobject_cast<FontPreviewStack*>(source_widget);

		if (not source)
			return;

		if (not source->syncContent())
			return;

		foreach(FontPreviewStack *preview, font_previews) {
			if (preview == source)
				continue;

			if (preview->syncContent())
				preview->editor()->copyTextFrom(source->editor());
		}
	}


	void FontPreviewsView::removeAllFontPreviews() {
		if (temporary_preview)
			removeTemporaryView();

		while (not font_previews.empty())
			remove(0);
	}

	void FontPreviewsView::remove(int id) {
		//qDebug() << "Closing font preview" << id;

		if (font_previews.empty())
			return;

		if ((id < 0) or (id >= font_previews.size()))
			return;

		if (focusedPreview() == font_previews[id])
			m_last_focused_preview = NULL;

		font_previews[id]->close();
		font_previews.removeAt(id);

		if (font_previews.empty())
			emit lastFontPreviewRemoved();
	}

	void FontPreviewsView::add(const QString &family) {

		if ((temporary_preview) and (temporary_preview->family() == family)) {
			fixateTemporaryView();
			return;
		}

		FontPreviewStack *preview = new FontPreviewStack(this);
		connect(preview, SIGNAL(previewCloseRequested(QWidget*)), SLOT(notifyPreviewRemoveRequest(QWidget*)));

		connect(preview, SIGNAL(previewContentChanged(QWidget*)), SLOT(updateAllPreviews(QWidget*)));
		connect(preview, SIGNAL(previewContentChanged(QWidget*)), SIGNAL(previewContentChanged()));

		// Update formatting buttons when text is selected or cursor moves around in any of the preview areas.
		connect(preview->editor(), SIGNAL(selectionBold(bool)), SIGNAL(selectionBold(bool)));
		connect(preview->editor(), SIGNAL(selectionItalic(bool)), SIGNAL(selectionItalic(bool)));
		connect(preview->editor(), SIGNAL(selectionUnderlined(bool)), SIGNAL(selectionUnderlined(bool)));
		connect(preview->editor(), SIGNAL(selectionSize(const QString&)), SIGNAL(selectionSize(const QString&)));

		connect(preview->editor(), SIGNAL(formattingChanged()), SIGNAL(formattingChanged()));

		if (empty())
			setDefaultPreviewText(preview->editor());
		else
			preview->editor()->copyTextFrom(font_previews[0]->editor());

		preview->setFont(family);

		previews_layout->addWidget(preview);
		font_previews.append(preview);

		emit fontPreviewAdded();
	}

	void FontPreviewsView::setDefaultPreviewText(FontEditorPreview *preview_widget) {
		Settings settings;
		preview_widget->setHtml(settings.defaultText());
		preview_widget->resizeAllText(defaultFontSize());
	}
}
