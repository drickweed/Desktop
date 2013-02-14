#pragma once

/** QT headers **/
#include <QAbstractTableModel>
#include <QStringList>
#include <QHash>

namespace FontMate {

	class Folders : public QAbstractTableModel {
		Q_OBJECT

		public:
			Folders(QObject *parent=0);

			/** QT Api **/
			int rowCount(const QModelIndex &parent=QModelIndex()) const;

			int columnCount(const QModelIndex &parent=QModelIndex()) const;

			QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

			QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

			Qt::ItemFlags flags(const QModelIndex &index) const;

			bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

			QStringList paths() const { return m_folders; }

			int indexOfPath(const QString &path) const;

			QString folder(QModelIndex &index) const;

			void setFolders(QStringList paths);

		signals:
			void blankFolderAdded(int row);

		public slots:
			void insertFolder(const QString &folder);

			int addFolder(const QString &folder);

			int addFolders(QStringList folders);

			int removeFolder(int row);

			void clear();

			void removeFoldersWithoutFonts();

			void updateFontCount(int index);

			void updateFontCounts();

		private:
			QStringList m_folders;
			QHash<QString, int> m_font_count;
	};

}
