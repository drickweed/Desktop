#include "pdfcommandsmodel.h"

/** QT headers **/
#include <QFile>
#include <QStringList>

/** JB headers **/
#include <jb/qt4/strings.h>

/** Application headers **/
#include "util.h"

using namespace JB;

PdfCommandsModel::PdfCommandsModel(QObject *parent):
		QAbstractListModel(parent) {
}

QByteArray PdfCommandsModel::pdfFromCommands(int count) const {
	QByteArray data;
	foreach(QByteArray segment, m_commands.mid(0, count + 1)) {
		data += segment;
		data += '\n';
	}

	return data;
}

int PdfCommandsModel::rowCount(const QModelIndex & /* parent */) const {
	return m_commands.count();
}

QVariant PdfCommandsModel::data(const QModelIndex &index, int role) const {
	QByteArray cleaned_up;
	switch (role) {
		case Qt::DisplayRole:
			return QVariant(String::printable(m_commands[index.row()]));
			break;
		default:
			break;
	};

	return QVariant();
}

void PdfCommandsModel::load(QString path) {
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	QByteArray data = file.readAll();
	setCommands(data);
	file.close();
	reset();
}

void PdfCommandsModel::setCommands(QByteArray data) {
	m_commands = data.split('\n');
	//m_commands.removeLast();
}

QByteArray PdfCommandsModel::command(int row) const {
	return m_commands[row];
}
