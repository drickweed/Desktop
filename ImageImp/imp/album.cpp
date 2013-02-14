#include "album.h"

/** Application headers **/
#include <storage/get.h>
#include <storage/set.h>

namespace ImageImp {

	Album::Album(QString name):
		RelationalObject() {
		setName(name);
	}

	void Album::setDefaults() {
		RelationalObject::setDefaults();
		setName("New album");
	}

	Template Album::pictureTemplate() const {
		return Template(albumPictureTemplate(id()), Template::PictureTemplate);
	}

	void Album::setPictureTemplate(QString picture_template) {
		setAlbumPictureTemplate(id(), picture_template);
	}

	void Album::setAlbumTemplate(QString album_template) {
		setAlbumAlbumTemplate(id(), album_template);
	}

	Template Album::albumTemplate() const {
		return Template(albumAlbumTemplate(id()), Template::AlbumTemplate);
	}


	Template Album::getTemplate(Template::TemplateType template_type) const {
		switch(template_type) {
			case Template::AlbumTemplate:
				return albumTemplate();
				break;
			case Template::PictureTemplate:
				return pictureTemplate();
				break;
		}

		// Should never reach here. all cases are handled above.
		// This line is only here to prevent warning in compiler
		return albumTemplate();
	}

	void Album::setTemplate(Template::TemplateType template_type, QString template_name) {
		switch(template_type) {
			case Template::AlbumTemplate:
				setAlbumTemplate(template_name);
				break;
			case Template::PictureTemplate:
				setPictureTemplate(template_name);
				break;
		}
	}

	QStringList ImageImp::Album::pictures() const {
		return albumImagePaths(id());
	}

}

void ImageImp::Album::removeDependencies() {
	deleteRelated("pictures");
	deleteRelated("photodates");
	deleteRelated("picture_templates");
	deleteRelated("album_templates");
}
