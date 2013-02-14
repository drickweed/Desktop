#include "tweaksmodel.h"

TweaksModel::TweaksModel(std::vector<Tweak> data, QObject *parent) : Model(parent) {
	QList<QVariant> rootData;
	// NOTE 3rd Tweak Description description column is for searchability
	rootData << "Description" << "File" << "Tweak Description";
	rootItem = new RootItem(rootData);
	std::vector<Tweak>::iterator itr = data.begin();
	while (itr != data.end()) {
		TweakItem *tItem = new TweakItem(*itr, rootItem);
		rootItem->appendChild(static_cast<ModelItem*>(tItem));
		++itr;
	}
}

void TweaksModel::clearSelection() {
	for (int i = 0; i < rootItem->childCount(); i++) {
		TweakItem *tItem = static_cast<TweakItem*>(rootItem->child(i));
		if (tItem->isCheckable(0)) {
			tItem->setState(0, false);
		} else {
			for (int j = 0; j < tItem->childCount(); j++) {
				TweakActionItem *_tItem = static_cast<TweakActionItem*>(tItem->child(j));
				_tItem->setState(0,false);
			}
		}
	}
	emit reset();
}

std::vector<TweakAction> TweaksModel::getSelected() {
	std::vector<TweakAction> checked;
	for (int i = 0; i < rootItem->childCount(); i++) {
		TweakItem *tItem = static_cast<TweakItem*>(rootItem->child(i));
		if (tItem->isCheckable(0)) { //no children
			if (tItem->state(0))
				checked.push_back(tItem->tweak().actions()[0]);
		} else { //has children
			for (int j = 0; j < tItem->childCount(); j++) {
				TweakActionItem *_tItem = static_cast<TweakActionItem*>(tItem->child(j));
				if (_tItem->state(0))
					checked.push_back(_tItem->action());
			}
		}
	}
	return checked;
}
