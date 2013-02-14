#pragma once

#include <jb/database/view.h>

using namespace JB;

namespace ImageImp {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class AlbumsView : public RelationalView {
		Q_OBJECT
		public:
			AlbumsView(QWidget *parent=0);
			~AlbumsView();

		public slots:
			virtual void setupHeaders();
	};

}
