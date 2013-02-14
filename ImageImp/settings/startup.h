#pragma once

namespace ImageImp {

	void ensureFunctionalState();

	/**
		Make sure all the folders required for the application to function exist and are accessible.

		Throws an exception on error.
	*/
	void ensureDataFolders();


	/**
		Copy default templates from resources to template folders.
	*/
	void createDefaultTemplates();

	void createPhotoshopScripts();
}
