#pragma once

#include <jb/database/view.h>

using namespace JB;

namespace ImageImp {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class GroupsView : public RelationalView {
		Q_OBJECT
		public:
			GroupsView(QWidget *parent=0);
			~GroupsView();

		public slots:
			virtual void setupHeaders();
	};

}
