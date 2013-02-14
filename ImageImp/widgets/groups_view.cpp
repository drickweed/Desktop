#include "groups_view.h"

#include "model/groups.h"

using namespace JB;

namespace ImageImp {

	GroupsView::GroupsView(QWidget *parent):
		RelationalView(parent) {
	}

	GroupsView::~GroupsView() {
	}

	void GroupsView::setupHeaders() {
		RelationalView::setupHeaders();

		header->setSectionHidden(Groups::PARENT_ID, true);
		header->hide();
		verticalHeader()->hide();
	}

}
