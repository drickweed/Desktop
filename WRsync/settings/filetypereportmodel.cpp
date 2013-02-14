#include "filetypereportmodel.h"

/** QT headers **/
#include <QDir>
#include <QFileInfo>
#include <QtAlgorithms>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** Application headers **/
#include <wrsync/sourcemanager.h>
#include <wrsync/logfile.h>

using namespace JB;

FileTypeReportModel::FileTypeReportModel(QObject *parent):
	QAbstractTableModel(parent),
	m_abort(false) {
}

QVariant FileTypeReportModel::data ( const QModelIndex & index, int role) const {
	if (not index.isValid())
		return QVariant();

	int row = index.row();
	int column = index.column();

	switch(role) {
		case Qt::DisplayRole:
			switch (column) {
				case 0: return QVariant(m_extensions[row]);
				case 1: return QVariant(m_report[m_extensions[row]]);
			}
			break;
		default:
			break;
	}

	return QVariant();
}

void FileTypeReportModel::sort ( int column, Qt::SortOrder order ) {

}

QVariant FileTypeReportModel::headerData ( int section, Qt::Orientation orientation, int role) const {
	switch (orientation) {
		case Qt::Horizontal:
			switch (section) {
				case 0: return QVariant(tr("File type"));
				case 1: return QVariant(tr("Size"));
			}

			break;
		default:
			break;
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

void FileTypeReportModel::setSources(QStringList sources) {
	SourceManager sm;
	sm.setSources(sources);
	m_sources = sm.paths();
}

void FileTypeReportModel::refresh() {
	m_extensions.clear();
	m_report.clear();

	foreach(QString source, m_sources) {
		if (not processFolder(source))
			break;
	}

	m_extensions = m_report.keys();
	qSort(m_extensions.begin(), m_extensions.end());

	reset();
}

int FileTypeReportModel::rowCount ( const QModelIndex & /* parent */) const {
	return m_extensions.count();
}

int FileTypeReportModel::columnCount ( const QModelIndex & /* parent */) const {
	return 2;
}


bool FileTypeReportModel::processFolder(QString root_path_name) {
	QDir path(root_path_name);
	LAPP_ << "Processing " << QSTR(root_path_name);

	foreach(QString file_name, path.entryList(QStringList() << "*", QDir::Files)) {
		if (m_abort)
			break;

		QString full_path = Path::join(root_path_name, file_name);
		QFileInfo file(full_path);
		LAPP_ << "File " << QSTR(file.absoluteFilePath());
		QString suffix = file.completeSuffix();
		m_report[suffix] += file.size();
	}

	foreach(QString path_name, path.entryList(QStringList() << "*", QDir::Dirs | QDir::NoDotAndDotDot)) {
		if (m_abort)
			break;

		processFolder(Path::join(root_path_name, path_name));
	}

	return true;
}
