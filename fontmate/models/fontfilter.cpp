#include "fontfilter.h"

/** QT headers **/
#include <QBrush>
#include <QColor>
#include <QApplication>

namespace FontMate {

	FontFilter::FontFilter(QObject *parent):
		QAbstractProxyModel(parent),
		m_search_result_count(0),
		m_show_only_selected(false),
		m_displayed_fonts_count(100) {
		setFilterRole(Qt::UserRole);

	}

	void FontFilter::setFilterFixedString(const QString &search_text) {
		m_search_text = search_text;
		resetShownFontCount();
		processRowFilter();
	}

	int FontFilter::rowCount(const QModelIndex & /* parent */) const {
		int count = filteredRowCount();
		if (count <= displayedFontCount())
			return count;
		return displayedFontCount() + 1;
	}

	bool FontFilter::showMoreItem(const QModelIndex &index) const {
		return (not allFontsShown()) and (index.isValid()) and (index.row() == rowCount() - 1);
	}

	QModelIndex FontFilter::mapFromSource(const QModelIndex &sourceIndex) const {
		return index(m_accepted_indexes.indexOf(sourceIndex.row()), sourceIndex.column());
	}

	QModelIndex FontFilter::mapToSource(const QModelIndex &proxyIndex) const {
		if (m_accepted_indexes.empty())
			return QModelIndex();

		return fontsModel()->index(m_accepted_indexes[proxyIndex.row()], proxyIndex.column());
	}

	QModelIndex FontFilter::index(int row, int column, const QModelIndex & /*parent */) const {
		return createIndex(row, column);
	}

	QModelIndex FontFilter::parent(const QModelIndex & /* child */) const {
		return QModelIndex();
	}

	QModelIndex FontFilter::showMoreIndex() const {
		if (allFontsShown())
			return QModelIndex();
		return index(rowCount()-1, 0);
	}

	void FontFilter::showMoreItems() {
		QModelIndex show_more = showMoreIndex();
		if (not show_more.isValid())
			return;

		int r = 0;

		m_displayed_fonts_count += 100;
		int total_font_count = filteredRowCount();
		if (m_displayed_fonts_count > total_font_count) {
			m_displayed_fonts_count = total_font_count;
			r = 1;
		}

		emit dataChanged(show_more, show_more);
		beginInsertRows(QModelIndex(), show_more.row()+1, m_displayed_fonts_count - r);
		endInsertRows();

	}

	void FontFilter::invalidate() {
		processRowFilter();
	}

	void FontFilter::resetShownFontCount() {
		m_displayed_fonts_count = 100;
	}

	QVariant FontFilter::data(const QModelIndex &index, int role) const {
		if (not index.isValid())
			return QVariant();

		if (showMoreItem(index)) {
			switch (role) {
				case Qt::BackgroundRole:
					return QVariant(QBrush(QColor(190, 210, 255)));
				default:
					break;
			}
			return QVariant();
		}

		return fontsModel()->data(mapToSource(index), role);
	}

	bool FontFilter::filterAcceptsRow(int source_row) {

		QModelIndex sourceIndex = fontsModel()->index(source_row, 0);

		bool search_filter = (searchText().isEmpty()) or (fontsModel()->data(sourceIndex, filterRole()).toString().contains(searchText(), Qt::CaseInsensitive));

		bool selection_filter = (not m_show_only_selected) or (fontsModel()->familySelected(source_row) or fontsModel()->familyMarked(source_row));

		if (search_filter)
			m_search_result_count++;

		return selection_filter and search_filter;

	}

	void FontFilter::processRowFilter() {
		m_accepted_indexes.clear();

		m_search_result_count = 0;

		int source_row_count = totalFontCount();
		for(int i=0; i<source_row_count; i++) {
			if (filterAcceptsRow(i))
				m_accepted_indexes.append(i);
		}

		if (searchFiltered())
			emit searchResultsChanged(m_search_result_count);
		else
			emit searchResultsChanged(-1);

		reset();
	}

	void FontFilter::setSourceModel(QAbstractItemModel *sourceModel) {
		if (this->sourceModel()) {
			disconnect(this->sourceModel(), SIGNAL(modelReset()), this, SLOT(invalidate()));
			disconnect(this->sourceModel(), SIGNAL(modelReset()), this, SLOT(resetShownFontCount()));
			disconnect(this->sourceModel(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)));
		}

		QAbstractProxyModel::setSourceModel(sourceModel);
		processRowFilter();
		connect(sourceModel, SIGNAL(modelReset()), SLOT(invalidate()));
		connect(sourceModel, SIGNAL(modelReset()), SLOT(resetShownFontCount()));
		connect(sourceModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)));
	}

	void FontFilter::showOnlySelected(int state) {
		switch(state) {
			case Qt::Unchecked:
				m_show_only_selected = false;
				break;
			default:
				m_show_only_selected = true;
				break;
		}
		processRowFilter();
	}
}
