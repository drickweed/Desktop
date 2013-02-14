#include "pdfgenerator.h"

/** QT headers **/
#include <QFile>

/** Application headers **/
#include <graphics/parser.h>
#include "util.h"
#include "settings.h"

using namespace Pdf::Util;

const QString stream_start = ">>\nstream\n";

PdfGenerator::PdfGenerator() {
}

QByteArray PdfGenerator::fromData(QByteArray data) {
	QFile header(Settings().dataFolder() + "pdf-header.txt");
	if (not header.open(QIODevice::ReadOnly)) {
		throw new ErrorOpeningPdfDataFiles;
	}

	QFile footer(Settings().dataFolder() + "pdf-footer.txt");
	if (not footer.open(QIODevice::ReadOnly)) {
		throw new ErrorOpeningPdfDataFiles;
	}

	QByteArray pdf_data = header.readAll();
	pdf_data += QString::number(data.size());
	pdf_data += stream_start;
	pdf_data += data;
	pdf_data += footer.readAll();

	header.close();
	footer.close();

	return pdf_data;

}
