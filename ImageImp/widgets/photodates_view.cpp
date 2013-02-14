#include "widgets/photodates_view.h"

#include "model/photodates.h"

using namespace JB;

namespace ImageImp {

	PhotographedPeopleView::PhotographedPeopleView(QWidget *parent):
		RelationalView(parent) {
	}

	void PhotographedPeopleView::setupHeaders() {
		RelationalView::setupHeaders();
		header->setSectionHidden(PhotographedPeopleModel::ALBUM_ID, true);
		header->setSectionHidden(PhotographedPeopleModel::DATETIME, true);
	}

}
