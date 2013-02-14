#pragma once

/** QT headers **/
#include <QList>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QMatrix>
#include <QTransform>
#include <QObject>
#include <QMap>

/** Application headers **/
#include "textnode.h"

/*
	Minimal pdf parser. Currently knows about:
	- Transformations
*/
class PdfParser : public QObject {
	Q_OBJECT
	public:

		enum SegmentType { Unknown, Command, Numeric };

		PdfParser(QObject *parent=0);
		~PdfParser();

		QTransform currentTransformation() const { return transformation.first(); }

		void applyTransformation(QTransform new_transformation);

		/*
			Parse one command including its arguments from the data.
			The command string is removed from the data.

			@param data Pdf graphics command data stream
			@return the number of arguments popped from the data
		*/
		int parseNext(QByteArray *data);

		QString makeReadable(QByteArray data);
		void init();

	private:


		QList<QTransform> transformation;
		QList<double> numeric_parameters;
		QStringList reference_parameters;
		QStringList parameters;
		QString command;

		QByteArray m_data;
		QByteArray raw_stream;

		static QStringList known_pdf_commands;
		static QMap<QString, QString> pdf_command_translation;
		static QStringList drawing_commands;

		static void loadKnownPdfCommands();
		static void loadPdfCommandTranslation();

		QString readableCommand() const;

		bool runCurrentCommand();
		QString makeReadableCommand();

		QList<double> toDeviceSpace(QList<double> parameters);

		QTransform parameterMatrix() const;
		QPointF parameterPoint() const;
		QRectF parameterRect() const;

		// PDF actions
		void saveState();
		void restoreState();

		// Brackets
		QStringList tree_brackets;
		int treeLevel() const { return tree_brackets.count(); }

		TextNode text_node;
		void initTextNode();

	signals:
		void transformationChanged(QTransform t);
		void textGenerated(TextNode t);

};
