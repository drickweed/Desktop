#pragma once

/** QT headers **/
#include <QAbstractTableModel>
#include <QMap>
#include <QDateTime>

/** Application hedaers **/
#include <wrsync/result.h>

class JobStateModel : public QAbstractTableModel {
	Q_OBJECT
	public:
		JobStateModel(QObject *parent);

		/** QT Api **/
		QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

		/** QT Api **/
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

		/** QT api **/
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

		/** QT Api **/
		int columnCount ( const QModelIndex & parent = QModelIndex() ) const { return 2; }

	public slots:
		void loadDayResults(QDate day);

	private:
		QList<Result> m_results;

};

