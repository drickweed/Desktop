#pragma once

#include <QAbstractTableModel>
#include <QStringList>
#include <QMap>

class FileTypeReportModel : public QAbstractTableModel {
	Q_OBJECT
	public:
		FileTypeReportModel(QObject *parent=0);

		/** QT api **/
		QVariant data ( const QModelIndex & index, int role=Qt::DisplayRole ) const;

		/** QT api **/
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

		/** QT api **/
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

		/** QT api **/
		int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

		/** QT api **/
		void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

		void refresh();

		void setSources(QStringList sources);

	private:
		bool processFolder(QString folder);
		QString extension(QString path) { return path.right(path.lastIndexOf('.')); }

	private:
		QStringList m_sources;
		QMap<QString, int> m_report;
		QList<QString> m_extensions;
		bool m_abort;
};

