#pragma once

/** QT headers **/
#include <QAbstractItemModel>
#include <QFont>
#include <QList>

/** Application headers **/
#include "glyph.h"

namespace FontMate {

	class GlyphsCompareModel : public QAbstractItemModel {
		Q_OBJECT
		public:
			GlyphsCompareModel(QObject *parent = 0);

			int rowCount(const QModelIndex &parent=QModelIndex()) const;
			int columnCount(const QModelIndex &parent=QModelIndex()) const;
			QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
			Qt::ItemFlags flags(const QModelIndex &index) const;
			bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

			void add(QFont font, QChar character);
			void add(Glyph glyph);

		signals:

		public slots:

		private:
			QList<Glyph> m_glyphs;

	};

}
