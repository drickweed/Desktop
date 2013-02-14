#pragma once

#include <QSortFilterProxyModel>

#include "filechecksummodel.h"

namespace Checksum {
	enum DisplayFilterBits { Valid = 0x1, Invalid = 0x2, Missing = 0x4, Unknown = 0x8 };
}

namespace MiniSFV {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FileChecksumModelProxy : public QSortFilterProxyModel {
		Q_OBJECT

		public:
			FileChecksumModelProxy(QObject *parent=0);

			virtual ~FileChecksumModelProxy() {}

			virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

			virtual void setSourceModel(QAbstractItemModel *sourceModel);

			/**
				Change record filter.

				@param bits bits to change
				@param state new state for bits
			*/
			void changeFilter(uint bits, bool state);

			uint displayFilter() const { return m_display_filter; }

		public slots:
			/**
				Filter to only show errors.
			*/
			void showOnlyErrors();

			/**
				Filter to show all records.
			*/
			void showAll();

			/**
				Set display filter.

				@param filter display filter.
			**/
			void setDisplayFilter(uint filter);

		private:
			uint m_display_filter; /// Record display filter bits (See Checksum::DisplayFilterBits)

			FileChecksumModel* model; /// To avoid constantly casting the model back to the actual type
	};
}

