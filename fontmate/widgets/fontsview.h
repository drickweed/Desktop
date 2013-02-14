#pragma once

/** QT headers **/
#include <QTableView>

/** Application headers **/
#include <models/fonts.h>
#include <models/fontfilter.h>
#include "fontitemdelegate.h"

namespace FontMate {

	/**
		List of fonts.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class FontsView : public QTableView {
		Q_OBJECT
		public:
			FontsView(QWidget *parent=0);

			virtual ~FontsView();

			Fonts *fonts() const;

			FontFilter *fontsProxy() const {return qobject_cast<FontFilter*>(model()); }

			/** QT Api **/
			void setModel(QAbstractItemModel *model);

		protected slots:
			/** QT Api **/
			virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

		protected:
			/** QT Api **/
			virtual void paintEvent(QPaintEvent *event);

		private:
			FontItemDelegate *font_view;

		signals:
			void selectedFontHighlighted(const QString &family);
			void unselectedFontHighlighted(const QString &family);
	};

}
