#include "template.h"

/** Application headers **/
#include "settings/system.h"
#include "storage/get.h"

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** QT headers **/
#include <QFileInfo>

using namespace JB;

namespace ImageImp {

	Template::Template(QString name, TemplateType template_type):
		m_type(template_type),
		m_name(name) {
	}

	QString Template::path() const {
		switch (type()) {
			case AlbumTemplate:
				return Path::join(System::albumTemplatesPath(), fileName());
				break;
			case PictureTemplate:
				return Path::join(System::pictureTemplatesPath(), fileName());
				break;
		}

		// Control should never reach here
		return "";
	}

	QString Template::name() const {
		return m_name;
	}

	Template::TemplateType Template::type() const {
		return m_type;
	}

	QString Template::fileName() const {
		return templateFileName(name());
	}

	void Template::setFileName(QString file_name) {
		m_name = QFileInfo(file_name).baseName();
	}

	QString Template::defaultAlbumTemplate() {
		return Path::join(System::albumTemplatesPath(), templateFileName(albumTemplates()[0]));
	}

	QString Template::defaultPictureTemplate() {
		return Path::join(System::pictureTemplatesPath(), templateFileName(pictureTemplates()[0]));
	}

	QString Template::templateFileName(QString template_name) {
		return QString("%1.psd").arg(template_name);
	}

}
