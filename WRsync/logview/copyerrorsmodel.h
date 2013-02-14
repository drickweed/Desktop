#pragma once

/** QT headers **/
#include <QAbstractListModel>
#include <QStringList>

class CopyErrorsModel : public QAbstractListModel {
	Q_OBJECT
	public:
		CopyErrorsModel(QObject *parent=0);

		void loadFromLog(QString log_filename);

		/** QT api **/
		Qt::ItemFlags flags ( const QModelIndex & index ) const;

		/** QT api **/
		QVariant data ( const QModelIndex & index, int role=Qt::DisplayRole ) const;

		/** QT api **/
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

	signals:

	public slots:

	private:
		QStringList m_errors;

};

