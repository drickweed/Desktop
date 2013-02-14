#pragma once

#include <QAbstractTableModel>

class ObjectPropertyModel : public QAbstractTableModel {
	Q_OBJECT
	public:
		ObjectPropertyModel(QObject *parent=0);
};


