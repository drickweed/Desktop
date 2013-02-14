#pragma once

/** QT headers **/
#include <QStringList>

/** JB headers **/
#include <jb/database/object.h>

/** Application headers **/
#include "imp/group.h"
#include "imp/template.h"

using namespace JB;

namespace ImageImp {

	/**
		Album.

		An album is a collection of people with photos.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Album : public RelationalObject {
		Q_OBJECT
		RELATIONAL_OBJECT(Album);

		SIMPLE_PROPERTY(QString, name, setName, m_name);
		Q_PROPERTY(QString name READ name WRITE setName)

		SIMPLE_PROPERTY(int, group_id, setGroupId, m_group_id);
		Q_PROPERTY(int group_id READ group_id WRITE setGroupId)
		BELONGS_TO(Group, group, setGroup);

		public:

			/**
				Convenience constructor.

				@param name album name
				@param parent QObject parent
			*/
			Album(QString name);

			virtual ~Album() {}

			/** JB Api **/
			void setDefaults();
			QString table() const { return "albums"; }
			QString fieldName() const { return "album"; }
			void removeDependencies();

			Template getTemplate(Template::TemplateType template_type) const;

			void setTemplate(Template::TemplateType template_type, QString template_name);

			Template pictureTemplate() const;

			void setPictureTemplate(QString picture_template);

			Template albumTemplate() const;

			void setAlbumTemplate(QString album_template);

			QStringList pictures() const;

	};
}
