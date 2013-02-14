#include "folders.h"

/** QT headers **/
#include <QDebug>
#include <QDir>

/** Application headers **/
#include <fonts/fontutils.h>

namespace FontMate {

	Folders::Folders(QObject *parent):
		QAbstractTableModel(parent) {
	}

	int Folders::rowCount(const QModelIndex & /* parent */) const {
		return m_folders.count();
	}

	void Folders::setFolders(QStringList paths) {
		removeSystemFontPaths(paths);
		m_folders = paths;
		reset();
	}

	int Folders::addFolders(QStringList folders) {
		int added_folder_count = 0;
		int current_folder_count = rowCount();

		removeSystemFontPaths(folders);

		foreach(QString folder, folders) {
			if (not m_folders.contains(folder, Qt::CaseInsensitive)) {
				m_folders << folder;
				added_folder_count++;
			}
		}
		if (added_folder_count > 0) {
			beginInsertRows(QModelIndex(), current_folder_count, current_folder_count + added_folder_count);
			endInsertRows();
		}
		return added_folder_count;
	}

	int Folders::columnCount(const QModelIndex & /* parent */) const {
		return 2;
	}

	QVariant Folders::headerData(int section, Qt::Orientation orientation, int role) const {

		switch (role) {
			case Qt::DisplayRole:
				switch (orientation) {
					case Qt::Horizontal:
						switch(section) {
							case 0: return QVariant(tr("Fonts"));
							case 1: return QVariant(tr("Path"));
						}
					default:
						break;
				}
			default:
				break;
		}

		return QVariant();
	}

	QVariant Folders::data(const QModelIndex &index, int role) const {
		if (not index.isValid())
			return QVariant();

		int column = index.column();
		int row = index.row();

		//qDebug() << "Model folder row" << row;

		switch (role) {
			case Qt::DisplayRole:
				if (column == 0) {
					if (m_font_count.contains(m_folders[row]))
						return QVariant(m_font_count[m_folders[row]]);
					return QVariant(QString());
				}
				if (column == 1)
					return QVariant(QDir::toNativeSeparators(m_folders[row]));
				break;
			default:
				break;
		}

		return QVariant();
	}

	Qt::ItemFlags Folders::flags(const QModelIndex & /* index */) const {
		// NOTE: Invalid indexes are special items. Let them be selectable and enabled
		// if (not index.isValid())
		//	return QAbstractTableModel::flags(index);

		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}

	QString Folders::folder(QModelIndex &index) const {
		if (not index.isValid())
			return QString();

		return m_folders[index.row()];
	}

	void Folders::updateFontCount(int row) {
		m_font_count[m_folders[row]] = fontCount(m_folders[row]);
		emit dataChanged(index(row, 0), index(row, 0));
	}

	void Folders::updateFontCounts() {
		for(int i=0; i<m_folders.count(); i++) {
			if (not m_font_count.contains(m_folders[i]))
				updateFontCount(i);
		}
	}

	bool Folders::setData(const QModelIndex &index, const QVariant &value, int role) {
		if (not index.isValid())
			return false;

		int column = index.column();
		int row = index.row();

		switch (role) {
			case Qt::EditRole:
				if (column == 0) {
					m_folders[row] = value.toString();
					emit dataChanged(index, index);
					return true;
				}
				break;
			default:
				break;
		}

		return QAbstractTableModel::setData(index, value, role);
	}

	int Folders::indexOfPath(const QString &path) const {
		return m_folders.indexOf(path);
	}

	void Folders::insertFolder(const QString &folder) {
		beginInsertRows(QModelIndex(), 0, 0);
		m_folders.prepend(folder);
		endInsertRows();
	}

	int Folders::addFolder(const QString& folder) {
		beginInsertRows(QModelIndex(), rowCount(), rowCount());
		m_folders.append(folder);
		endInsertRows();
		return rowCount();
	}

	int Folders::removeFolder(int row) {
		beginRemoveRows(QModelIndex(), row, row);
		m_font_count.remove(m_folders[row]);
		m_folders.removeAt(row);
		endRemoveRows();

		if (row > 0)
			return row-1;

		if (rowCount() == 0)
			return -1;

		return row;
	}

	void Folders::clear() {
		m_folders.clear();
		reset();
	}

	void Folders::removeFoldersWithoutFonts() {
		for (int i=m_folders.count()-1; i >= 0; i--)
			if (not folderContainsFonts(m_folders[i]))
				m_folders.removeAt(i);

		reset();
	}

}

