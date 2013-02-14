#pragma once

#include <vector>
#include "model.h"
#include "version.h"

/**
	Model for representing available applications.
*/
class ApplicationsModel : public Model {
	Q_OBJECT

	public:

		/**
			Constructor for ApplicationsModel.
			@param data Vector of application to display.
			@param parent Parent for Qt heirarchy and memory management.
		*/
		ApplicationsModel(const std::vector<Application> data, QObject *parent = 0);

		/**
			Get all versions that are "checked".
			@return vector of selected versions
		*/
		std::vector<Version> getSelected();

		std::vector<Version> getLastVersions();

		/**
			Sets the provided versions as "checked".
			@param vers versions to select.
		*/
		std::vector<QModelIndex> setSelected(std::vector<Version> vers);
		/**
			Wipes and repopulates model.
			@param data Vector of applications to display.
		*/
};
