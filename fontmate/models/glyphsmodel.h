#pragma once

/** QT headers **/
#include <QAbstractTableModel>
#include <QFont>
#include <QList>

/** Application headers **/
#include "glyph.h"

class GlyphsModel : public QAbstractTableModel {
	Q_OBJECT
	public:
		GlyphsModel(QObject *parent=0);

		void setFont(QFont font) {
			m_font = font;
		}

		QFont font() const { return m_font; }

		int rowCount(const QModelIndex &parent=QModelIndex()) const;

		int columnCount(const QModelIndex &parent=QModelIndex()) const;

		QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

		//Qt::ItemFlags flags(const QModelIndex &index) const;

		//bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

	private:
		QFont m_font;

	signals:

	public slots:

};
