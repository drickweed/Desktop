#pragma once

#include <QModelIndex>
#include <QColor>
#include <string>

#include "application.h"
#include "version.h"
#include "tweak.h"

/**
	Contains information about a single peice of information in the model.
*/
class Cell {
	public:
		/**
			Provides information wether field is checkable or not
		*/
		bool checkable;
		/**
			Stores the current check state. Only for checkable fields.
		*/
		bool state;
		/**
			Field's text.
		*/
		std::string text;

		/**
			Default constructor.
			@param text Cell's text.
			@param checkable Wether cell is checkable or not.
			@param state Initial state.
		*/
		Cell(std::string text="", bool checkable=false, bool state=false):
			checkable(checkable),
			state(state),
			text(text) {
			}
};

/**
	A model item.
*/
class ModelItem {
	public:
		/**
			Default constructor.
			@param parent Parent for Qt heirarchy and memory management.
		*/
		ModelItem(ModelItem *parent=0);

		/**
			Default deconstructor.

		*/
		virtual ~ModelItem();

		/**
			Item type.

			@return item type
		*/
		virtual int type() const { return 0; }

		/**
			Append child to item.
			@param child child to append.
		*/
		void appendChild(ModelItem *child);

		/**
			@param row child row.
			@return child item.
		*/
		ModelItem *child(int row) const {return m_child_items.value(row);}

		/**
			@return number of child rows.
		*/
		int childCount() const {return m_child_items.count();}

		/**
			@return amount of cells in item.
		*/
		virtual int columnCount() const {return m_cells.size();}

		/**
			@param column column of desired information.
			@return displayable data
		*/
		virtual QVariant data(int column) const;

		/**
			@return item's row
		*/
		int row() const;

		/**
			@return item's parent
		*/
		ModelItem *parent() const { return m_parent;}

		/**
			@param col column to check.
			@return checkable state.
		*/
		bool isCheckable(int col) const;

		/**
			Set check state for a certain column.
			@param col column to modify.
			@param st new status.
		*/
		void setState(int col, bool st);

		/**
			Get check state.
			@return state.
		*/
		bool state(int col) const;

		/**
			Turns off all checks.
		*/
		void reset();

		virtual QVariant toolTip() const { return QVariant(); }
		virtual QVariant color() const { return QVariant(); }
		virtual QVariant backgroundColor() const { return QVariant(); }

	protected:
		/**
			List of cells.
		*/
		QList<Cell> m_cells;
		/**
			List of child items.
		*/
		QList<ModelItem*> m_child_items;
		/**
			Pointer to parent item.
		*/
		ModelItem *m_parent;
};

/**
	Application tree root item.
	Strores headers.
*/
class RootItem : public ModelItem {
	private:
		/**
			List of header titles.
		*/
		QList<QVariant> m_data;

	public:
		/**
			Default constructor.
			@param data List of header titles.
			@param parent Parent for Qt heirarchy and memory management.
		*/
		RootItem(const QList<QVariant> &data, ModelItem *parent=0);

		/**
			@return amount of headers.
		*/
		int columnCount() const { return m_data.count(); }

		/**
			@param column Column of desired header
			@return header data.
		*/
		QVariant data(int column) const { return m_data.value(column); }
};

/**
	Application item
*/
class ApplicationItem : public ModelItem {
		/**
			Application instance the item represents.
		*/
		Version m_version;
		Application *m_application;

	public:
		/**
			Default constructor.
			@param app Application to display.
			@param parent Parent for Qt heirarchy and memory management.
		*/
		ApplicationItem(Application app, ModelItem *parent=0);
		/**
			Gets intallable object for item.
			@return Last version of represented Application.
		*/
		Version version() const { return m_version; }
		/**
			Gets view color.
			@return color.
		*/
		QVariant color() const { return version().hasInstaller() ? QVariant() : QVariant(QColor(Qt::red)); }
		QVariant backgroundColor() const { return version().silent() ? QVariant() : QVariant(QColor(Qt::lightGray)); }
// 		QVariant toolTip() const;
};

/**
	Application version
*/
class VersionItem : public ModelItem {
		/**
			Version instance the item represents.
		*/
		Version m_version;

	public:
		/**
			Default constructor.
			@param version Version to display.
			@param parent Parent for Qt heirarchy and memory management.
		*/
		VersionItem(Version version, ModelItem *parent = 0);

		/**
			Gets intallable object for item.
			@return Item's version.
		*/
		Version version() const { return m_version; }
		/**
			Gets view color.
			@return color.
		*/
		QVariant color() const { if (!(version().hasInstaller())) return QVariant(QColor(Qt::red)); else return QVariant(); }
		QVariant backgroundColor() const { return version().silent() ? QVariant() : QVariant(QColor(Qt::lightGray)); }
};

class TweakItem : public ModelItem {
		Tweak m_tweak;
	public:
		TweakItem(Tweak tweak, ModelItem *parent = 0);
		Tweak tweak() { return m_tweak; }
};

class TweakActionItem : public ModelItem {
		TweakAction m_action;
	public:
		TweakActionItem(TweakAction action, ModelItem *parent = 0);
		TweakAction action() { return m_action; }
};
