#include "pdfobjectsmodel.h"

/** QT headers **/
#include <QFile>
#include <QByteArray>
#include <QStringList>
#include <QTemporaryFile>
#include <QDebug>
#include <QFileInfo>

/** Zlib headers **/
#include <zlib.h>

/** Application headers **/
#include "inflate.h"
#include "util.h"
#include <objects/dictionary.h>

const QString STREAM_START = "stream\n";
const QString STREAM_END = "endstream\n";
const int START_OF_DATA = 0;
const int INVALID_INDEX = -1;
const int ENDOBJECT_SIZE = 6;
const char COMMENT_SIGN = '%';
const char NEWLINE = '\n';
const char SPACE = ' ';

using namespace Pdf;

PdfObjectsModel::PdfObjectsModel(QObject *parent):
	QAbstractListModel(parent) {
}

void PdfObjectsModel::load(QString filename) {
	QFile pdf_file(filename);
	pdf_file.open(QIODevice::ReadOnly);
	loadPdf(pdf_file.readAll());
	pdf_file.close();
}

void PdfObjectsModel::loadPdf(QByteArray data) {
	m_data = data;
	extractObjects();
}

QByteArray PdfObjectsModel::stripComments(QByteArray data) {
	QByteArray stripped_data;
	foreach(QByteArray line, data.split(NEWLINE)) {
		if (line.startsWith(COMMENT_SIGN))
			continue;
		stripped_data.append(line + NEWLINE);
	}
	return stripped_data;
}

int PdfObjectsModel::rowCount(const QModelIndex & parent) const {
	return m_objects.count();
}

QVariant PdfObjectsModel::data(const QModelIndex & index, int role) const {
	int row = index.row();
	switch (role) {
		case Qt::DisplayRole:
			return m_objects.keys()[row];
			break;
		default:
			break;
	}
	return QVariant();
}

int PdfObjectsModel::objectId(QByteArray data) const {
	QByteArray header = data.left(data.indexOf("obj"));
	QList<QByteArray> header_parts = header.split(' ');
	bool conversion_result;
	int object_id = header_parts[0].toInt(&conversion_result, 10);
	return object_id;
}

QByteArray PdfObjectsModel::objectContent(QByteArray data) const {
	QByteArray content = data.mid(data.indexOf("obj")+3);
	return content.trimmed();
}

QString PdfObjectsModel::printableDictionary(QByteArray data) const {
	QString printable;
	QMap<QString, QString> dictionary = Dictionary::parseFromData(data);

	foreach(QString key, dictionary.keys()) {
		printable.append(QString("%1: %2\n").arg(key).arg(dictionary[key]));
	}
	return printable;
}

QByteArray PdfObjectsModel::object(int object_id) const {
	return m_objects[object_id];
}

QString PdfObjectsModel::printableObject(int object_id) const {
	return printableDictionary(m_objects[object_id]);
}

qint32 PdfObjectsModel::objectId(int idnex) const {
	return m_objects.keys()[idnex];
}

void PdfObjectsModel::extractObjects() {
	QList<QByteArray> objects;
	QList<QByteArray> parts;
	QByteArray p_data= stripComments(m_data);
	QByteArray line;
	QByteArray part;
	int index;

	while (not p_data.isEmpty()) {
		index = p_data.indexOf("endobj");
		if (index == INVALID_INDEX) {
			p_data.clear();
			continue;
		}

		part = p_data.left(index);
		objects.append(part);
		p_data.remove(0, part.size() + ENDOBJECT_SIZE);
	}

	qDebug() << "Found" << objects.size() << "objects";
	foreach(QByteArray obj, objects) {
		m_objects[objectId(obj)] = objectContent(obj);
	}

	reset();
}

QByteArray PdfObjectsModel::extractStream(QByteArray object) {
	int starting_search_position = START_OF_DATA; // Index in pdf byte data where last stream was found.


	// Find next stream
	int found_stream_index = object.indexOf(STREAM_START, starting_search_position);

	if (found_stream_index == INVALID_INDEX)
		return QByteArray();

	int end_of_stream = object.indexOf(STREAM_END, found_stream_index);

	if (end_of_stream == INVALID_INDEX)
			return QByteArray();

	int stream_length = end_of_stream - found_stream_index;

	// Extract stream
	QByteArray stream = object.mid(found_stream_index, stream_length);

	QTemporaryFile compressed_file;
	QTemporaryFile extracted_file;
	compressed_file.write(stream);
	compressed_file.close();
	extracted_file.close();

	int result = inflate(compressed_file.fileName(), extracted_file.fileName());

	if (result != Z_OK)
		qDebug() << "Error" << result << "extracting data from" << compressed_file.fileName();

	extracted_file.open();
	return extracted_file.readAll();

}

bool PdfObjectsModel::isCatalog(int object_id) const {

}

bool PdfObjectsModel::isPage(int object_id) const {

}

bool PdfObjectsModel::isContent(int object_id) const {

}

QString PdfObjectsModel::objectType(int object_id) const {

}

bool PdfObjectsModel::isDictionaty(int object_id) const {
	return m_objects[object_id].startsWith("<<");
}
