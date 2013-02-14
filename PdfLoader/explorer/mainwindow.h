#pragma once

#include "ui_mainwindow.h"

/** QT headers **/
#include <QMainWindow>

/** Extra widget headers **/
#include <propertyeditor.h>

/** Application headers **/
#include "widgets/pdfcommandsview.h"
#include "pdf/pdfcommandsmodel.h"
#include "pdf/pdfscene.h"
#include "graphics/parser.h"
#include "pdf/pdfdocument.h"
#include "pdf/pdfobjectsmodel.h"

using namespace Pdf;

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

		void loadFile(QString file_name);

	private:
		PdfCommandsView *pdf_commands_view, *pdf_objects_view;
		PdfCommandsModel *pdf_commands;
		PdfScene *pdf_scene;
		PdfDocument *document;
		Graphics::Parser *parser;
		PropertyEditor::PropertyEditor *property_editor;
		PdfObjectsModel *pdf_objects;

		void setupUi();
		void setupModels();
		void connectUi();

		void applyHighlights(QImage *image);

		bool highlightSelectedLine() const { return highlightLineCheckBox->isChecked(); }
		int selectedLine() const { return lineSpinBox->value(); }
		QRectF selectedLineRect() const { return document->lineBoundingRect(selectedLine()); }

	private slots:
		void showOpenFileDialog();
		void updateDocument(int index);
		void updateLineSelector();
		void showPage();
		void parsePage();
		void saveAsReadable();
		void loadObjectInViewer(int index);

		void updateTransform(QTransform t);
		void processPdfText(TextNode t);

	signals:
		void documentUpdated();

	protected:
		void changeEvent(QEvent *e);
};

