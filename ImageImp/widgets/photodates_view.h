#pragma once

#include <jb/database/view.h>

using namespace JB;

namespace ImageImp {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class PhotographedPeopleView : public RelationalView {
		Q_OBJECT
		public:
			PhotographedPeopleView(QWidget *parent=0);

		public slots:
			virtual void setupHeaders();
	};

}
