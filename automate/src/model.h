#pragma once

#include <QAbstractItemModel>

#include "modelitems.h"
/**
	Implementation of most common methods required to create a model.
*/
class Model : public QAbstractItemModel {
	Q_OBJECT

	public:
		/**
			Default constructor.
			@param parent Parent for Qt heirarchy and memory management.
		*/
		Model(QObject *parent = 0);
		/**
			Default deconstructor.
		*/
		~Model();

		/**
			Refer to Qt4 docs.
		*/
		QVariant data(const QModelIndex &index, int role) const;
		/**
			Refer to Qt4 docs.
		*/
		Qt::ItemFlags flags(const QModelIndex &index) const;
		/**
			Refer to Qt4 docs.
		*/
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		/**
			Refer to Qt4 docs.
		*/
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		/**
			Refer to Qt4 docs.
		*/
		QModelIndex parent(const QModelIndex &index) const;
		/**
			Refer to Qt4 docs.
		*/
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		/**
			Refer to Qt4 docs.
		*/
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
		/**
			Refer to Qt4 docs.
		*/
		bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

		/**
			Clears all selected checkmarks.
		*/
		void clearSelection();

	protected:
		/**
			Top level tree element.
		*/
		ModelItem *rootItem;

	signals:

		/**
			Item checked
		*/
		void itemChecked(void *item);

		/**
			Item unchecked
		*/
		void itemUnChecked(void *item);
};

