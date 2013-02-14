#include <QtGui>
#include <QStringList>
#include <vector>

#include "model.h"

Model::Model(QObject *parent) :
	QAbstractItemModel(parent) {
}

Model::~Model() {
	delete rootItem;
}

int Model::columnCount(const QModelIndex &parent) const {
	if (parent.isValid())
		return static_cast<ModelItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant Model::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	ModelItem *item = static_cast<ModelItem*>(index.internalPointer());

	switch (role) {
		case Qt::DisplayRole:
			return item->data(index.column());
		case Qt::CheckStateRole:
			if (item->isCheckable(index.column())) {
				if (item->state(index.column()))
					return Qt::Checked;
				else
					return Qt::Unchecked;
			}
		case Qt::ToolTipRole:
			return item->toolTip();
		case Qt::ForegroundRole:
			return item->color();
		case Qt::BackgroundRole:
			return item->backgroundColor();
		default:
			break;
	}
	return QVariant();
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return 0;
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (static_cast<ModelItem*>(index.internalPointer())->isCheckable(index.column()))
		flags |= Qt::ItemIsUserCheckable;
	return flags;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	ModelItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ModelItem*>(parent.internalPointer());

	ModelItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &index) const {
	if (!index.isValid())
		return QModelIndex();

	ModelItem *childItem = static_cast<ModelItem*>(index.internalPointer());
	ModelItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int Model::rowCount(const QModelIndex &parent) const {
	ModelItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ModelItem*>(parent.internalPointer());

	return parentItem->childCount();
}

bool Model::setData( const QModelIndex &index, const QVariant& /* value */, int role) {
	if (!index.isValid())
		return false;
	if (role == Qt::CheckStateRole) {
		ModelItem *item = static_cast<ModelItem*>(index.internalPointer());
		if (item->isCheckable(index.column())) {
			if (item->state(index.column())) {
				item->setState(index.column(),false);
				emit itemUnChecked(NULL);
			}
			else {
				item->setState(index.column(),true);
				emit itemChecked(NULL);
			}
			emit dataChanged(index, index);
		}
		return true;
	}
	return false;
}

void Model::clearSelection() {
	for (int i=0; i < rootItem->childCount(); ++i) {
		ModelItem* child = rootItem->child(i);
		child->reset();
		for (int j=0; j < child->childCount(); ++j) {
			child->child(j)->reset();
		}
	}
	emit reset(); //collapses, need another signal
}
