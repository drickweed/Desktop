#pragma once

/** QT headers **/
#include <QStringListModel>
#include <QList>

/** JB headers **/
#include <jb/database/object_ref.h>
#include <jb/std/property.h>

/** STL headers **/
#include <exception>

/** Application headers **/
#include <imp/album.h>

using namespace JB;
using namespace std;

namespace ImageImp {

	class ErrorSavingTemplateSelection : public exception {

	};

	class ErrorLoadingTemplateSelection : public exception {

	};

	/**
		Templates.
	*/
	class TemplatesModel: public QStringListModel {
		Q_OBJECT

		public:
			TemplatesModel(QString path, Template::TemplateType template_type, QObject *parent=0);

			/**
				Populate from valid template files in path.

				@param path Path to get template files from.
			*/
			void populate(QString path);

			virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

			virtual Qt::ItemFlags flags(const QModelIndex &index) const;

			inline const QString filename(const QModelIndex index) const;

			const QString fullPath(const QModelIndex index) const;

			const QString path() const { return m_path; }

			const QModelIndex findByTemplateName(const QString template_name) const;

			void setAlbum(RelationalObjectRef album);

			bool hasValidAlbum() const { return album_id != RelationalObject::invalidId(); }

			Template::TemplateType templateType() const { return m_template_type; }

			int albumId() const { return album_id; }

		private:
			QString m_path;
			Template::TemplateType m_template_type;

			int album_id;

		signals:
			void albumChanged(int album_id);

	};

}
