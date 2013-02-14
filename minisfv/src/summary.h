#pragma once

/** QT headers **/
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QModelIndex>

/** Application headers **/
#include "filechecksummodel.h"
#include "statussummaryitem.h"

namespace MiniSFV {

	/**
		Result summary.

		Acts as a view on the model.

		View and filter results.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Summary : public QWidget {
		Q_OBJECT
		public:
			/**
				Constructor.

				@param model model
				@param parent parent
			*/
			Summary(FileChecksumModel *model, QWidget *parent = 0);

		signals:
			void displayFilter(uint filter);

		public slots:

			void update();
			void updateDisplayFilter();
			void checkOnlyErrorFilters();
			void checkOnlyValidFilter();

		private:
			QColor default_color; /// Default button color

			StatusSummaryItem *valid,		/// Valid checksum
							  *missing,		/// Missing file
		  					  *invalid,		/// Invalid checksum
							  *unknown;		/// File was not checked
			QHBoxLayout *top_layout;
			QVBoxLayout *valid_layout,
						*invalid_layout;
			FileChecksumModel *model;

			void updateValid(unsigned int count, uint total);
			void updateInvalid(unsigned int count);
			void updateMissing(unsigned int count);
			void updateUnchecked(unsigned int count);
	};

}

