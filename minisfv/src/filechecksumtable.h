#pragma once

/** QT headers **/
#include <QWidget>
#include <QTableView>
#include <QHeaderView>

/** Application headers **/
#include <filechecksummodel.h>
#include <filechecksummodelproxy.h>

namespace MiniSFV {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FileChecksumTable : public QTableView {
		Q_OBJECT

		public:
			FileChecksumTable(QWidget *parent=0);

		private:
			unsigned int m_selected_row;

			FileChecksumModel* sourceModel() const;
			FileChecksumModelProxy* proxyModel() const;

		public slots:
			void forwardedDropEvent(QDropEvent *event);
			void scrollToArea(QModelIndex start, QModelIndex end);

		private slots:
			void contextMenu(const QPoint &point);
			void removeSelected();
			void itemActivated(const QModelIndex &index);

		signals:
			void itemSelected(const QModelIndex &index);

	};

}
