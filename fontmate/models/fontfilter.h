#pragma once

/** QT headers **/
#include <QAbstractProxyModel>
#include <QFont>
#include <QList>

/** Application headers **/
#include "fonts.h"

namespace FontMate {

	/**
		Filter model implementing:
		- Filter by search
		- Filter by quantity: progressively shows more and more fonts in the list according to user request
		- Selection

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FontFilter : public QAbstractProxyModel {
		Q_OBJECT

		public:
			FontFilter(QObject *parent=0);

			/** QT Api **/
			virtual void setSourceModel(QAbstractItemModel *sourceModel);

			/** QT Api **/
			virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;

			/** QT Api **/
			virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

			/** QT Api **/
			virtual int columnCount(const QModelIndex & /* parent */=QModelIndex()) const {
				return fontsModel()->columnCount();
			}

			/** QT Api **/
			virtual QModelIndex mapFromSource ( const QModelIndex & sourceIndex ) const;

			/** QT Api **/
			virtual QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;

			/** QT Api **/
			virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

			/** QT Api **/
			virtual QModelIndex parent(const QModelIndex &child) const;

			bool empty() const { return rowCount() == 0; }

			bool showingSelected() const { return m_show_only_selected; }

			bool showMoreItem(const QModelIndex &index) const;

			void showMoreItems();

			QString searchText() const { return m_search_text; }

			void setFilterRole(Qt::ItemDataRole role) { m_filter_role = role; }

			Qt::ItemDataRole filterRole() const { return m_filter_role; }

			int displayedFontCount() const { return m_displayed_fonts_count; }

			int totalFontCount() const { return fontsModel()->rowCount(); }

		public slots:
			void showOnlySelected(int state);

			void resetShownFontCount();

			void setFilterFixedString(const QString &search_text);

			void invalidate();

		private:
			int m_search_result_count;

			Fonts* fontsModel() const { return static_cast<Fonts*>(sourceModel()); }

			bool m_show_only_selected; ///< Show only selected fonts

			bool filtersActive() const { return m_show_only_selected or (not searchText().isEmpty()); }

			int m_displayed_fonts_count;

			bool allFontsShown() const { return displayedFontCount() >= filteredRowCount(); }

			QString m_search_text;

			QList<int> m_accepted_indexes;

			Qt::ItemDataRole m_filter_role;

			void processRowFilter();

			int filteredRowCount() const { return m_accepted_indexes.count(); }

			bool searchFiltered() const {
				return not searchText().isEmpty();
			}

			bool fontsFiltered() const {
				return m_accepted_indexes.count() != fontsModel()->rowCount();
			}

			QModelIndex showMoreIndex() const;

		signals:
			void searchResultsChanged(int result_count);

		protected:
			virtual bool filterAcceptsRow(int source_row);

		};

}
