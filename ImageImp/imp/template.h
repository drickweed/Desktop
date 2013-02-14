#pragma once

/** QT headers **/
#include <QString>

/** STL headers **/
#include <exception>

namespace ImageImp {

	class NoTemplateAvailable : public std::exception {

	};

	class Template {

		public:

			enum TemplateType {AlbumTemplate, PictureTemplate};

			Template(QString name, TemplateType template_type);

			/**
				Full template path.

				@note Does not check if file exists.

				@return full template path.
			*/
			QString path() const;

			QString name() const;

			TemplateType type() const;

			QString fileName() const;

			void setFileName(QString file_name);

			static QString defaultAlbumTemplate();

			static QString defaultPictureTemplate();

			static QString templateFileName(QString template_name);

		private:
			TemplateType m_type;
			QString m_name;
	};

}
