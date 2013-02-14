#include "people_view.h"

#include "model/people.h"

using namespace JB;

namespace ImageImp {

	PeopleView::PeopleView(QWidget *parent):
		RelationalView(parent) {
	}

	PeopleView::~PeopleView() {
	}

	void PeopleView::setupHeaders() {
		RelationalView::setupHeaders();

		header->setSectionHidden(People::GROUP_ID, true);
		header->resizeSection(People::NAME, 200);
		header->resizeSection(People::SID, 100);

		verticalHeader()->hide();
	}

}
