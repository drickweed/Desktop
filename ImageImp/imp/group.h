#pragma once

/** QT headers **/
#include <QString>

/** JB headers **/
#include <jb/database/object.h>

namespace ImageImp {

	/**
		Group.

		A group is a collection of people who can have multiple photo albums.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Group : public JB::RelationalObject {
		Q_OBJECT
		RELATIONAL_OBJECT(Group);

		SIMPLE_PROPERTY(QString, name, setName, m_name);
		Q_PROPERTY(QString name READ name WRITE setName)

		public:

			/**
				Convenience constructor.

				@param name group name
				@param parent QObject parent
			*/
			Group(QString name);

			virtual ~Group() {}

			/** JB Api **/
			void setDefaults();
			QString table() const { return "groups"; }
			QString fieldName() const { return "group"; }
			void removeDependencies();
	};
}
