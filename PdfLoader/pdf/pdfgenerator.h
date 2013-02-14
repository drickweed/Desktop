#pragma once

/** QT headers **/
#include <QByteArray>
#include <QStringList>

/** STL headers **/
#include <exception>

#include "settings.h"

using namespace std;

class ErrorOpeningPdfDataFiles : public exception {};

/*
	Generates a valid pdf file stream from a list of pdf graphics drawing commands.
*/
class PdfGenerator {
    public:
        PdfGenerator();

		/*
			Generate vaild PDF file for consumption by poppler.

			@param data text representation of pdf graphics commands.
		*/
		QByteArray fromData(QByteArray data);
};
