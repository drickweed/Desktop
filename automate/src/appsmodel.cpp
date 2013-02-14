#include <vector>

#include "appsmodel.h"
#include "application.h"
#include <jb/std/memory.h>

ApplicationsModel::ApplicationsModel(std::vector<Application> data, QObject *parent):
	Model(parent) {
	QList<QVariant> rootData;
	rootData << "Title" << "Version" << "Description" << "Url" << "";
	rootItem = new RootItem(rootData);
	std::vector<Application>::iterator itr = data.begin();
	while (itr != data.end()) {
		ApplicationItem *aItem = new ApplicationItem(*itr, rootItem);
		rootItem->appendChild(static_cast<ModelItem*>(aItem));
		++itr;
	}
}

std::vector<Version> ApplicationsModel::getSelected() {
	std::vector<Version> versions;
	for (int i = 0; i < rootItem->childCount(); ++i)
	{
		ApplicationItem* app = static_cast<ApplicationItem*>(rootItem->child(i));
		if (app->state(1))
			versions.push_back(app->version());
		for (int j = 0; j < app->childCount(); ++j)
		{
			VersionItem* ver = static_cast<VersionItem*>(app->child(j));
			if (ver->state(1))
				versions.push_back(ver->version());
		}
	}
	return versions;
}

std::vector<Version> ApplicationsModel::getLastVersions() {
	std::vector<Version> lasts;
	for(int i = 0; i < rootItem->childCount(); ++i) {
		ApplicationItem* app = static_cast<ApplicationItem*>(rootItem->child(i));
		lasts.push_back(app->version());
	}
	return lasts;
}

//FIXME: like worst implementation evar. what if i pass name+version only?
std::vector<QModelIndex> ApplicationsModel::setSelected(std::vector<Version> vers) {

	std::vector<QModelIndex> toExpand;
	std::vector<Version>::iterator itr = vers.begin();
	while (itr != vers.end()) {
		for (int i = 0; i < rootItem->childCount(); ++i) {
			ApplicationItem* app = static_cast<ApplicationItem*>(rootItem->child(i));
			//FIXME: i feel like a retard.
			if (app->version().name() == itr->name()) {
				if (itr->is_last) {
					app->setState(1, true);
					break;
				} else 	{
					for (int j = 0; j < app->childCount(); ++j) {
						VersionItem* ver = static_cast<VersionItem*>(app->child(j));
						if (ver->version().version()  == itr->version()) {
							ver->setState(1,true);
							toExpand.push_back(index(i,0));
							break;
						}
					}
				}
			}
		}
		++itr;
	}
	return toExpand;
}
