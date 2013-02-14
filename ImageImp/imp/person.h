#pragma once

/** QT headers **/
#include <QString>

/** JB headers **/
#include <jb/database/object.h>

/** Application headers **/
#include "imp/group.h"

using namespace JB;

namespace ImageImp {

	/**
		Person.

		Has many pictures
		Belongs to a single group.
		Appears in all albums.
		Can have multiple pictures in each album.
		Has one default picture per album.

		@todo Add an option for a person not to appear in specific albums.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Person : public RelationalObject {
		Q_OBJECT
		RELATIONAL_OBJECT(Person);

		SIMPLE_PROPERTY(QString, name, setName, m_name);
		Q_PROPERTY(QString name READ name WRITE setName)

		SIMPLE_PROPERTY(QString, sid, setSid, m_sid);
		Q_PROPERTY(QString sid READ sid WRITE setSid)

		SIMPLE_PROPERTY(int, group_id, setGroupId, m_group_id);
		Q_PROPERTY(int group_id READ group_id WRITE setGroupId)
		BELONGS_TO(Group, group, setGroup);

		public:

			/**
				Convenience constructor.

				@param name person full name
				@param parent QObject parent
			*/
			Person(QString name);

			/** JB Api **/
			void setDefaults();
			QString table() const { return "people"; }
			QString fieldName() const { return "person"; }
			void removeDependencies();

	};
}
