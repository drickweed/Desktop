#include "print.h"

/** QT headers **/
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>

#include "fontutils.h"

namespace FontMate {
	namespace Print  {
		void fontList(QStringList fonts, QWidget *parent, QString preview_text, int font_size) {
			QPrinter *printer = new QPrinter;
			QPrintDialog printDialog(printer, parent);

			if (printDialog.exec() == QDialog::Accepted) {
				QTextDocument *print_document = createFontListDocument(fonts, preview_text, font_size);
				print_document->print(printer);
				delete print_document;
			}

			delete printer;
		}

	};
}
