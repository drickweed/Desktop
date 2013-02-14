#pragma once

#include <QAbstractProxyModel>

#include "folders.h"

namespace FontMate {

	class FolderSelectionModel : public QAbstractProxyModel {
		Q_OBJECT

		public:
			FolderSelectionModel(QObject *parent=0);

			/** QT api **/
			int rowCount(const QModelIndex &parent=QModelIndex()) const;

			/** QT api **/
			int columnCount(const QModelIndex &parent = QModelIndex()) const;

			/** QT api **/
			QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

			/** QT api **/
			QModelIndex mapFromSource ( const QModelIndex & sourceIndex ) const;

			/** QT api **/
			QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;

			/** QT api **/
			QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

			/** QT api **/
			QModelIndex parent(const QModelIndex &child) const;

			int indexOfPath(const QString &path) const;

			/** QT api **/
			void setSourceModel(QAbstractItemModel *sourceModel);

			bool loadFolderItem(int index) const { return (index == (rowCount() - 2)); }

			bool loadPdfItem(int index) const { return (index == (rowCount() - 1)); }

		public slots:
			void force_reset() { reset(); }

		private:
			Folders *foldersModel() const { return qobject_cast<Folders*>(sourceModel()); }
	};

}
