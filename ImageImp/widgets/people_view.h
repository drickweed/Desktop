#pragma once

#include <jb/database/view.h>

using namespace JB;

namespace ImageImp {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class PeopleView : public RelationalView {
		Q_OBJECT
		public:
			PeopleView(QWidget *parent=0);
			~PeopleView();

		public slots:
			virtual void setupHeaders();
	};

}
