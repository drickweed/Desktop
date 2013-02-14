#include "folderselectionmodel.h"

namespace FontMate {

	const int specialItemCount = 3;

	FolderSelectionModel::FolderSelectionModel(QObject *parent):
		QAbstractProxyModel(parent) {
	}

	int FolderSelectionModel::rowCount(const QModelIndex & /* parent */) const {
		return foldersModel()->rowCount() + specialItemCount;
	}

	int FolderSelectionModel::columnCount(const QModelIndex & /* parent */) const {
		return foldersModel()->columnCount();
	}

	QModelIndex FolderSelectionModel::mapFromSource(const QModelIndex &sourceIndex) const {
		return index(sourceIndex.row()+1, sourceIndex.column());
	}

	QModelIndex FolderSelectionModel::mapToSource(const QModelIndex &proxyIndex) const {
		return foldersModel()->index(proxyIndex.row()-1, proxyIndex.column());
	}

	QVariant FolderSelectionModel::data(const QModelIndex &index, int role) const {
		if (not index.isValid())
			return QVariant();

		int row = index.row();
		int column = index.column();

		switch (role) {
			case Qt::DisplayRole:
				switch (column) {
					case 1:
						if (row == 0)
							return QVariant(tr("Installed fonts"));
						if (loadFolderItem(row))
							return QVariant(tr("Select folder..."));
						if (loadPdfItem(row))
							return QVariant(tr("Select PDF..."));
					default: break;
				}
				break;
			default:
				break;
		}

		return foldersModel()->data(mapToSource(index), role);
	}

	void FolderSelectionModel::setSourceModel(QAbstractItemModel *sourceModel) {
		QAbstractProxyModel::setSourceModel(sourceModel);

		connect(foldersModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(force_reset()));
		connect(foldersModel(), SIGNAL(rowsInserted(const QModelIndex&, int, int)), SLOT(force_reset()));
		connect(foldersModel(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)), SLOT(force_reset()));
	}

	int FolderSelectionModel::indexOfPath(const QString &path) const {
		int path_row = foldersModel()->indexOfPath(path);
		if (path_row != -1)
			path_row += 1;
		return path_row;
	}

	QModelIndex FolderSelectionModel::index(int row, int column, const QModelIndex & /* parent */) const {
		return createIndex(row, column);
	}

	QModelIndex FolderSelectionModel::parent(const QModelIndex & /* child */) const {
		return QModelIndex();
	}

}
