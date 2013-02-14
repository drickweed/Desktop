#include "mainwindow.h"

namespace ImageImp {

	void MainWindow::setupSearchEditors() {

		connect(groups_search_edit, SIGNAL(textEdited(const QString&)), document->groups_search_proxy, SLOT(setFilterFixedString(const QString&)));

		connect(albums_search_edit, SIGNAL(textEdited(const QString&)), document->albums_search_proxy, SLOT(setFilterFixedString(const QString&)));

		connect(people_search_edit, SIGNAL(textEdited(const QString&)), document->people_search_proxy, SLOT(setFilterFixedString(const QString&)));

		connect(unphotographed_people_search_edit, SIGNAL(textEdited(const QString&)), document->unphotographed_people, SLOT(setFilterFixedString(const QString&)));

	}

}
