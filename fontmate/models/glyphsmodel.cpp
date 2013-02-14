#include "glyphsmodel.h"

/** QT headers **/


GlyphsModel::GlyphsModel(QObject *parent) :
	QAbstractTableModel(parent) {
}

int GlyphsModel::rowCount(const QModelIndex &parent) const {
	return 5;
}

int GlyphsModel::columnCount(const QModelIndex &parent) const {
	return 5;
}

QVariant GlyphsModel::data(const QModelIndex &index, int role) const {

	if (not index.isValid())
		return QVariant();

	int row = index.row();
	int column = index.column();
	int unicode_char = row*column + column;

	switch (role) {
		case Qt::DisplayRole:
			return QVariant(QChar(unicode_char));
		default:
			break;
	}

	return QVariant();
}
