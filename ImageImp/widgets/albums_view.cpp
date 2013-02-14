#include "albums_view.h"

#include "model/albums.h"

using namespace JB;

namespace ImageImp {

	AlbumsView::AlbumsView(QWidget *parent):
		RelationalView(parent) {
	}

	AlbumsView::~AlbumsView() {
	}

	void AlbumsView::setupHeaders() {
		RelationalView::setupHeaders();

		header->setSectionHidden(Albums::GROUP_ID, true);
		header->setSectionHidden(Albums::LAST_IMAGE_PATH, true);
		header->hide();
		verticalHeader()->hide();
	}

}
