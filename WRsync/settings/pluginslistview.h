#pragma once

/** QT headers **/
#include <QListView>

class PluginsListView : public QListView {
	Q_OBJECT

	public:
		PluginsListView(QWidget *parent = 0);

protected:
		void currentChanged ( const QModelIndex & current, const QModelIndex & /** previous **/) {
			emit rowSelected(current);
		}

	signals:
		void rowSelected(QModelIndex index);

};
