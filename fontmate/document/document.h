#pragma once

/** Application headers **/
#include <models/fonts.h>
#include <models/fontfilter.h>
#include <models/glyphsmodel.h>
#include <folders/folders.h>
#include <folders/folderselectionmodel.h>

/** JB headers **/
#include <jb/models/tags.h>

using namespace JB;

namespace FontMate {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Document{
		public:
			Document();

			~Document();

			Fonts *fonts;
			FontFilter *font_search_proxy;
			Folders *folders;
			FolderSelectionModel *folder_selection_model;
			Tags *tags;
			GlyphsModel *glyphs;
	};

}
