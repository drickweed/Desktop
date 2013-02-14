#include "document.h"

#include <settings/settings.h>

namespace FontMate {

	Document::Document() {
		fonts = new Fonts;
		fonts->populateFrom(Fonts::BuiltinFontSource);

		font_search_proxy = new FontFilter;

		font_search_proxy->setSourceModel(fonts);
//		font_search_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

		folders = new Folders;
		folders->setFolders(Settings().fontPaths());
		folders->updateFontCounts();

		folder_selection_model = new FolderSelectionModel;
		folder_selection_model->setSourceModel(folders);

		tags = new Tags;

		glyphs = new GlyphsModel();
	}

	Document::~Document() {
		fonts->deleteLater();
		font_search_proxy->deleteLater();
		folders->deleteLater();
		folder_selection_model->deleteLater();
		tags->deleteLater();
		glyphs->deleteLater();
	}

}
