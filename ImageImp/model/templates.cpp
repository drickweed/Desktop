#include "templates.h"

/** QT headers **/
#include <QFileInfo>
#include <QDebug>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** Application headers **/
#include "storage/get.h"

using namespace JB;

namespace ImageImp {
	TemplatesModel::TemplatesModel(QString path, Template::TemplateType template_type, QObject *parent):
		QStringListModel(parent),
		m_template_type(template_type),
		album_id(RelationalObject::invalidId()){
		populate(path);
	}

	void TemplatesModel::populate(QString path) {
		m_path = path;
		setStringList(templateFiles(path));
		reset();
	}

	QVariant TemplatesModel::data(const QModelIndex & index, int role) const {

		if ((role == Qt::DisplayRole) and index.isValid())  {
			return QVariant(QFileInfo(filename(index)).baseName());
		}

		return QStringListModel::data(index, role);
	}

	Qt::ItemFlags TemplatesModel::flags(const QModelIndex & index) const {
		if (not index.isValid())
			return Qt::NoItemFlags;

		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}

	const QString TemplatesModel::filename(const QModelIndex index) const {
		return QStringListModel::data(index, Qt::DisplayRole).toString();
	}

	const QString TemplatesModel::fullPath(const QModelIndex index) const {
		return QFileInfo(Path::join(path(), filename(index))).absoluteFilePath();
	}

	const QModelIndex TemplatesModel::findByTemplateName(const QString template_name) const {

		QModelIndex found_index;

		QModelIndexList indexes = match(index(0,0), Qt::DisplayRole, QVariant(template_name), 1, Qt::MatchExactly);
		if (indexes.empty())
			return QModelIndex();
		else
			return indexes[0];
	}

	void TemplatesModel::setAlbum(RelationalObjectRef album) {
		album_id = album.id();

		emit albumChanged(album_id);
	}

}
