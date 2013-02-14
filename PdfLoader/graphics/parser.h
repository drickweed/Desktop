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

/** STL headers **/
#include <exception>

using namespace std;

class NotEnoughParametersForMatrix : public exception {};

class NotEnoughParametersForRectangle : public exception {};

class NotEnoughParametersForPoint : public exception {};

namespace Pdf {
	namespace Graphics {
		/*
			Minimal pdf graphics parser. Only parses the segment in the pdf file which represents the
			drawing commands. Currently knows about:
			- Transformations
			- Text
		*/
		class Parser : public QObject {
			Q_OBJECT
			public:

				enum SegmentType { Unknown, Command, Numeric };

				Parser(QObject *parent=0);
				~Parser();

				QTransform currentTransformation() const { return transformation.first(); }

				void applyTransformation(QTransform new_transformation);

				/*
					Parse one command including its arguments from the data.
					The command string is removed from the data.

					@param data Pdf graphics command data stream
					@return the number of arguments popped from the data
				*/
				int parseNext(QByteArray *data);

				/*
					All commands as human readable text.
				*/
				QString readableCommands(QByteArray data);

				/*
					Init parser.

					- Clears temporary storage.
				*/
				void init();

			private:

				QList<QTransform> transformation; // Transformation stack
				QString command; // Last found command

				QList<double> numeric_parameters;
				QStringList reference_parameters;
				QStringList parameters;

				QList<QByteArray> m_array;

				QByteArray m_data;
				QByteArray raw_stream;

				/* List of known pdf commands and a translation table for their readable names */
				static QStringList known_pdf_commands;
				static QMap<QString, QString> pdf_command_translation;
				static QStringList drawing_commands;

				static void loadKnownPdfCommands();
				static void loadPdfCommandTranslation();

				/*
					Human readable representation of the last parsed command.
				*/
				QString readableCommandName() const;

				/*
					Run the current command using available plugins and emitting the command using qt signal.
				*/
				bool runCommand();

				/*
					Human readable representation of the last parsed command, including its parameters.
				*/
				QString readableCommand();

				/*
					Convert coordinates to device space using current transformation matrix.
				*/
				QList<double> toDeviceSpace(QList<double> parameters);

				/*
				  Get matrix from current parameters
				*/
				QTransform parameterMatrix() const;

				/*
				  Get point from current parameters
				*/
				QPointF parameterPoint() const;

				/*
				  Get rectangle from current parameters
				*/
				QRectF parameterRect() const;

				/*
				  Parse an array structure.
				*/
				void parseArray(QByteArray array);

				// PDF actions
				void saveState();
				void restoreState();

				// Brackets
				QStringList tree_brackets;
				int treeLevel() const { return tree_brackets.count(); }
				bool inANode() const { return not tree_brackets.empty(); }

				TextNode text_node;
				void initTextNode();

			signals:
				void started();
				void transformationChanged(QTransform t);
				void textGenerated(TextNode t);

		};
	}
}
