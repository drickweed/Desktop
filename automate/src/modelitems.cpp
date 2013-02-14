#include "modelitems.h"

ModelItem::ModelItem(ModelItem *parent):
	m_parent(parent) {
}

ModelItem::~ModelItem() {
	qDeleteAll(m_child_items);
}

void ModelItem::appendChild(ModelItem *item) {
	m_child_items.append(item);
}

int ModelItem::row() const {
	if (parent())
		return parent()->m_child_items.indexOf(const_cast<ModelItem*>(this));
	return 0;
}

bool ModelItem::isCheckable(int col) const {
	if (col < 0 || col >= m_cells.size())
		return false;
	return m_cells[col].checkable;
}

void ModelItem::setState(int col, bool st) {
	if (col < 0 || col >= m_cells.size())
		return;
	m_cells[col].state = st;
}

bool ModelItem::state(int col) const {
	if (col < 0 || col >= m_cells.size())
		return false;
	return m_cells[col].state;
}

QVariant ModelItem::data(int col) const {
	if (col < 0 || col >= m_cells.size())
		return QVariant();
	return QVariant(m_cells[col].text.c_str());
}

void ModelItem::reset() {
	for (int i=0; i < m_cells.size(); ++i)
		m_cells[i].state = false;
}

RootItem::RootItem(const QList<QVariant> &data, ModelItem *parent):
	ModelItem(parent),
	m_data(data) { }

ApplicationItem::ApplicationItem(Application app, ModelItem* parent) :
	ModelItem(parent),
	m_version(app.lastVersion()),
	m_application(&app) {
	m_version.is_last = true;
	m_cells << Cell(app.name()) << Cell("["+m_version.version()+"]", true) << Cell(app.summary()) << Cell(m_version.url()) << Cell();
	std::vector<Version> versions = app.versions();
	std::vector<Version>::iterator itr = versions.begin();
	while (itr != versions.end()) {
		VersionItem *version = new VersionItem(*itr, this);
		appendChild(static_cast<ModelItem*>(version));
		itr++;
	}
}

// QVariant ApplicationItem::toolTip() const {
// 	return QVariant(QString(m_app->description().c_str()));
// }

VersionItem::VersionItem(Version version, ModelItem* parent) :
	ModelItem(parent),
	m_version(version) {
	m_cells << Cell() << Cell(version.version(), true) << Cell() << Cell(version.url()) << Cell(version.name());
}

TweakItem::TweakItem(Tweak tweak, ModelItem *parent) :
	ModelItem(parent) ,
	m_tweak(tweak) {
	std::vector<TweakAction> actions = tweak.actions();
	if (actions.size() == 1)
		m_cells << Cell(actions[0].description(), true, false) << Cell(actions[0].path());
	else if (actions.size() > 1) {
		m_cells << Cell(tweak.description());
		for (unsigned int i = 0; i < actions.size(); i++) {
			TweakActionItem *ta = new TweakActionItem(actions[i], this);
			appendChild(static_cast<ModelItem*>(ta));
		}
	}
}

TweakActionItem::TweakActionItem(TweakAction action, ModelItem *parent) :
	ModelItem(parent),
	m_action(action) {
	m_cells << Cell(action.description(), true, false) << Cell(action.path()) << Cell(static_cast<TweakItem*>(this->parent())->tweak().description());
}

