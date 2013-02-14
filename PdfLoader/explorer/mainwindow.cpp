#include "mainwindow.h"

/** QT headers **/
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTextStream>

/** JB headers **/
#include <jb/qt4/strings.h>

/** Application headers **/
#include "pdf/pdfgenerator.h"
#include "pdf/util.h"
#include "graphics/parser.h"

using namespace JB;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent) {
	setupModels();

	Ui::MainWindow::setupUi(this);
	setupUi();

	connectUi();
}

MainWindow::~MainWindow() {
	delete document;
}

void MainWindow::setupUi() {

	// Create widget
	pdf_commands_view = new PdfCommandsView(this);
	QVBoxLayout *vlayout = qobject_cast<QVBoxLayout*>(commandsSection->layout());
	vlayout->addWidget(pdf_commands_view);

	// Set model
	pdf_commands_view->setModel(pdf_commands);

	// Create widget
	property_editor = new PropertyEditor::PropertyEditor(this);
	property_editor->setObject(document);
	vlayout = qobject_cast<QVBoxLayout*>(dockWidgetContents->layout());
	vlayout->addWidget(property_editor);

	// Set model
	pdfView->setScene(pdf_scene);

	// Create widget
	pdf_objects_view = new PdfCommandsView(this);
	QHBoxLayout *hlayout = qobject_cast<QHBoxLayout*>(objectBrowserTab->layout());
	hlayout->insertWidget(0, pdf_objects_view);

	// Set model
	pdf_objects_view->setModel(pdf_objects);
}

void MainWindow::setupModels() {
	pdf_commands = new PdfCommandsModel(this);
	pdf_scene = new PdfScene(this);

	document = new PdfDocument(this);
	// FIXME read page dimentions from pdf file:
	document->setPageDim(QRectF(0, 0, 595.28, 841.89)); // A4 paper hardcoded. default in lilypond output.

	parser = new Graphics::Parser(this);

	pdf_objects = new PdfObjectsModel(this);
}

void MainWindow::connectUi() {
	connect(actionOpen, SIGNAL(triggered()), SLOT(showOpenFileDialog()));
	connect(pdf_commands_view, SIGNAL(rowSelected(int)), SLOT(updateDocument(int)));
	connect(pdf_objects_view, SIGNAL(rowSelected(int)), SLOT(loadObjectInViewer(int)));
	connect(this, SIGNAL(documentUpdated()), SLOT(showPage()));
	connect(this, SIGNAL(documentUpdated()), SLOT(parsePage()));
	connect(actionSave_as_readable, SIGNAL(triggered()), SLOT(saveAsReadable()));
	connect(highlightLineCheckBox, SIGNAL(toggled(bool)), SLOT(showPage()));

	connect(parser, SIGNAL(transformationChanged(QTransform)), SLOT(updateTransform(QTransform)));
	connect(parser, SIGNAL(textGenerated(TextNode)), SLOT(processPdfText(TextNode)));
}

void MainWindow::loadObjectInViewer(int index) {
	objectTextEdit->setText(pdf_objects->printableObject(pdf_objects->objectId(index)));
}

void MainWindow::updateTransform(QTransform t) {
	document->setTransform(t);
	property_editor->resetProperties();
}

void MainWindow::saveAsReadable() {
	QFile readable_file("readable_commands.txt");
	readable_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream out(&readable_file);
	out << readableTextEdit->document()->toPlainText();
	readable_file.close();
}

void MainWindow::updateDocument(int index) {

	document->load(PdfGenerator().fromData(pdf_commands->pdfFromCommands(index)));
	emit documentUpdated();
}

void MainWindow::processPdfText(TextNode t) {
	if (t.text == QByteArray("\1")) { // G-Key found
		document->addLine(t.position());
		updateLineSelector();
	}
}

void MainWindow::loadFile(QString file_name) {
	//pdf_commands->load(file_name);
	pdf_objects->load(file_name);
}

void MainWindow::applyHighlights(QImage *image) {
	QPainter painter(image);

	// Transform from document space to qt painter space
	QTransform t(1, 0, 0, -1, 0, document->pageDim().height());
	painter.setWorldTransform(t);

	if (highlightSelectedLine() && document->validLineNumber(selectedLine())) {
		QPen red_pen(QColor(Qt::red));
		painter.setPen(red_pen);
		painter.drawRect(document->lineBoundingRect(selectedLine()));
	}

	painter.end();
}

void MainWindow::showPage() {
	QImage page_image = document->renderPage(0);

	applyHighlights(&page_image);

	pdf_scene->setImage(page_image);
}

void MainWindow::updateLineSelector() {
	lineSpinBox->setMaximum(document->lineCount());
	lineSpinBox->setMinimum((document->lineCount()>0 ? 1 : 0));
	lineSpinBox->setValue(lineSpinBox->minimum());
}

void MainWindow::parsePage() {
	// Display entire pdf data as text in a viewer
	pdfDataView->setText(String::removeNulls(document->data()));

	parser->init();
	document->initParsedInfo();

	// Display parsed readable pdf commands
	int pdf_commands_count = pdf_commands_view->currentRow();
	readableTextEdit->setText(parser->readableCommands(pdf_commands->pdfFromCommands(pdf_commands_count)));
}

void MainWindow::showOpenFileDialog() {
	QString file_name = QFileDialog::getOpenFileName(this, tr("Select PDF commands stream"), "", "*.txt;;*.pdf");

	if (file_name.isEmpty())
		return;

	loadFile(file_name);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
