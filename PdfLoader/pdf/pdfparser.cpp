#include "pdfparser.h"

/** QT headers **/
#include <QFile>
#include <QTextStream>

/** STL headers **/
#include <iostream>

#include "util.h"

using namespace std;
using namespace Util;

QStringList PdfParser::known_pdf_commands;
QStringList PdfParser::drawing_commands = QStringList() << "m" << "l" << "re";
QMap<QString, QString> PdfParser::pdf_command_translation;

void PdfParser::loadKnownPdfCommands() {
	QFile pdf_commands("pdf_commands.txt");
	pdf_commands.open(QIODevice::ReadOnly);
	QTextStream in(&pdf_commands);
	known_pdf_commands = in.readLine().split(' ', QString::SkipEmptyParts);
	pdf_commands.close();
}

void PdfParser::loadPdfCommandTranslation() {
	QFile pdf_commands_table("pdf_commands_table.txt");
	pdf_commands_table.open(QIODevice::ReadOnly);
	QTextStream in(&pdf_commands_table);
	QStringList line_parts;
	while (not in.atEnd()) {
		line_parts = in.readLine().split(' ', QString::SkipEmptyParts);
		pdf_command_translation[line_parts[0]] = line_parts[1];
	}
	pdf_commands_table.close();
}

void PdfParser::init() {
	transformation.clear();
	// Initial transform matrix (1 0, 0 1, 0 0)
	transformation.append(QTransform());
}

PdfParser::PdfParser(QObject *parent):
	QObject(parent) {
	loadKnownPdfCommands();
	loadPdfCommandTranslation();
}

PdfParser::~PdfParser() {
}

QString PdfParser::makeReadable(QByteArray data) {
	QString readable;

	init();
	m_data = data; // Working on a copy of data since it will be modified during parsing.
	m_data.replace('\n',' ');
	while (not m_data.isEmpty()) {
		parseNext(&m_data);
		readable += makeReadableCommand();
		runCurrentCommand();
		m_data = m_data.trimmed();
	}

	return readable;
}

void PdfParser::applyTransformation(QTransform new_transformation) {
	transformation[0] = new_transformation * transformation[0];
}

void PdfParser::saveState() {
	transformation.push_front(transformation[0]);
}

void PdfParser::restoreState() {
	transformation.pop_front();
}

int PdfParser::parseNext(QByteArray *data) {
	int parameter_count=0;
	int index=0;
	bool command_found = false;
	bool done = false;
	bool conversion_result;
	QByteArray segment;
	QString value;
	double numeric_value;
	SegmentType type = Unknown;

	numeric_parameters.clear();
	parameters.clear();
	raw_stream.clear();

	while (not done) {

		if (data->isEmpty()) {
			done = true;
			continue;
		}

		QString first_char = QString(data->at(0));

		// Open brackets. Starting a tree node.
		if (OpenBrackets.contains(first_char)) {
			tree_brackets.push_front(first_char);
			data->remove(0, 1);
			continue;
		}

		// Close brackets. Leaving a tree node.
		if (CloseBrackets.contains(first_char)) {
			if (bracketsMatch(tree_brackets[0][0],first_char[0])) {
				tree_brackets.pop_front();
				data->remove(0, 1);
				continue;
			}
		}

		// Did we find a tree node ?
		if (treeLevel() > 0) {
			index = data->indexOf(CloseBrackets[OpenBrackets.indexOf(tree_brackets[0])]);
			raw_stream = data->left(index);
			data->remove(0, index);
		} else {

			// Get a segment
			index = data->indexOf(' ');

			if (index != -1) { // Whitespace separator found
				segment = data->left(index);
				data->remove(0, index+1); // +1 for the found whitespace, because we don't need it anymore.
			} else if (not data->isEmpty()) { //No whitespace selarator found. process the rest of the data as is.
				segment = *data;
				data->clear();
			}
		}

		// Convert data to string
		value = QString(segment);

		// Identify the segment
		if (known_pdf_commands.contains(value)) {
			type = Command;
			command_found = true;
			done = true;
			command = value;
			continue;
		}

		// Is this a number ?
		numeric_value = value.toDouble(&conversion_result);
		if (conversion_result) {
			type = Numeric;
			numeric_parameters.append(numeric_value);
			parameters.append(value);
			continue;
		}

		type = Unknown;
		parameters.append(value);
	}

	return parameter_count;
}

QString PdfParser::readableCommand() const {
	return pdf_command_translation.value(command, command);
}

QString PdfParser::makeReadableCommand() {
	QString command_string;
	QStringList text_arguments;

	if (drawing_commands.contains(command))
		numeric_parameters = toDeviceSpace(numeric_parameters);

	foreach(double parameter, numeric_parameters) {
		text_arguments.append(QString("%1").arg(parameter));
	}

	if (not raw_stream.isEmpty())
		text_arguments.append(printable(raw_stream));

	command_string = QString("%1 (%2)\n")
					.arg(readableCommand())
//					.arg(parameters.join(", "));
					.arg(text_arguments.join(", "));


	return command_string;
}

QList<double> PdfParser::toDeviceSpace(QList<double> parameters) {
	QList<double> result;
	QPointF point;
	while (not parameters.empty()) {
		point = QPointF(parameters[0], parameters[1]);
		parameters.removeFirst();
		parameters.removeFirst();
		point = currentTransformation().map(point);
		result.append(point.x());
		result.append(point.y());
	}
	return result;
}

void PdfParser::initTextNode() {
	text_node = TextNode();
}

QTransform PdfParser::parameterMatrix() const {
	return QTransform(numeric_parameters[0],
					 numeric_parameters[1],
					 numeric_parameters[2],
					 numeric_parameters[3],
					 numeric_parameters[4],
					 numeric_parameters[5]);
}

QPointF PdfParser::parameterPoint() const {
	return QPointF(numeric_parameters[0], numeric_parameters[1]);
}

QRectF PdfParser::parameterRect() const {
	QRectF rect;
	rect.setTopLeft(parameterPoint());
	rect.setSize(QSizeF(numeric_parameters[2], numeric_parameters[3]));
	return rect;
}

bool PdfParser::runCurrentCommand() {
	if (command == "q") {
		saveState();
		emit transformationChanged(currentTransformation());
		return true;
	}

	if (command == "Q") {
		restoreState();
		emit transformationChanged(currentTransformation());
		return true;
	}

	if (command == "cm") {
		applyTransformation(parameterMatrix());
		emit transformationChanged(currentTransformation());
		return true;
	}

	if (command == "BT") {
		initTextNode();
		return true;
	}

	if (command == "Tm") {
		text_node.applyMatrix(parameterMatrix());
		return true;
	}

	if (command == "Td") {
		text_node.applyDeltaMatrix(parameterPoint());
		return true;
	}

	if (command == "Tj") {
		text_node.text = raw_stream;
		emit textGenerated(text_node);
		return true;
	}

	return false;
}
