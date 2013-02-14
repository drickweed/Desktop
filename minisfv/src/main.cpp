/** QT headers **/
#include <QtGui>

/** STL headers **/
#include <iostream>

/** Application headers **/
#include "window.h"
#include "filechecksummodel.h"
#include "controller.h"

#ifndef __APPLE__
#include "getopt.h"
#endif

using namespace MiniSFV;
using namespace std;

void printSyntax() {
	cout << "minisfv [checksum filename] | [files to create checksum for]" << endl;
}

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);

#ifndef __APPLE__
    // @FIXME Getopt parsing not compatible with apple's implementation
	// Parse command line
	static struct option long_options[] = {
		{"help", 0,0,0},
		{0,0,0,0}
	};
	int option_index = 0;
	int c;

	while (true) {
		c = getopt_long(argc, argv, "", long_options, &option_index);

		// no more options:
		if (c == -1)
			break;

		// parse options
		switch (c) {
			case 0:
				// long options
				switch (option_index) {
					case 0: printSyntax();
							exit(0);
				}
		}
	}
#endif

	FileChecksumModel model;

	Controller controller;
	controller.setModel(&model);

	Window *window = new Window(&model);
	window->show();

	// Tasks
	/// Abort scanning
	QObject::connect(window->controls->abort, SIGNAL(abort()), &controller, SLOT(abort()));
	/// Skip current file
	QObject::connect(window->controls->abort, SIGNAL(skip()), &controller, SLOT(skipCurrent()));
	/// Create checksum file
	QObject::connect(window->controls, SIGNAL(createChecksumFile(QString)), &controller, SLOT(create(QString)));
	/// Verify checksums
	QObject::connect(window->controls->tasks, SIGNAL(verify()), &controller, SLOT(verify()));
	/// Checksum file loaded
	QObject::connect(&model, SIGNAL(loadedChecksumFile()), &controller, SLOT(verify()));
	/// Scan a single file
	QObject::connect(window->controls->file_table, SIGNAL(itemSelected(const QModelIndex&)), &controller, SLOT(verify(const QModelIndex&)));

	// Controller startedAction
	QObject::connect(&controller, SIGNAL(startedAction(int)), window->controls, SLOT(showAbort()));
	QObject::connect(&controller, SIGNAL(startedAction(int)), window->controls, SLOT(showProgressFileList()));
	QObject::connect(&controller, SIGNAL(startedAction(int)), window->controls, SLOT(showProgress()));
	QObject::connect(&controller, SIGNAL(startedAction(int)), window->controls->action_progress, SLOT(setMaximum(int)));

	// Controller finished
	QObject::connect(&controller, SIGNAL(finished()), window->controls, SLOT(showTasks()));
	QObject::connect(&controller, SIGNAL(finished()), window->controls, SLOT(showSummary()));
	QObject::connect(&controller, SIGNAL(finished()), window->controls->action_progress, SLOT(reset()));
	QObject::connect(&controller, SIGNAL(finished()), window->controls->summary, SLOT(update()));
	QObject::connect(&controller, SIGNAL(finished()), window->controls, SLOT(showEditorFileList()));

	QObject::connect(&model, SIGNAL(filesAdded()), window->controls, SLOT(showEditorFileList()));
	QObject::connect(&model, SIGNAL(recordCountChanged(int)), window->controls->tasks, SLOT(updateCreateActionAvailability(int)));

	// Results change file list view
	QObject::connect(&controller, SIGNAL(errorsFound()), &(window->controls->proxy), SLOT(showOnlyErrors()));
	QObject::connect(&controller, SIGNAL(allValid()), &(window->controls->proxy), SLOT(showAll()));
	QObject::connect(&controller, SIGNAL(errorsFound()), window->controls->summary, SLOT(checkOnlyErrorFilters()));
	QObject::connect(&controller, SIGNAL(allValid()), window->controls->summary, SLOT(checkOnlyValidFilter()));

	// Reset progress bar between porcesses
	QObject::connect(&controller, SIGNAL(finishedProcessingSingleRecord()), window->controls->file_progress, SLOT(reset()));

	QObject::connect(&model, SIGNAL(loadedChecksumFile()), window->controls->tasks, SLOT(showVerify()));

	// Close after creating checksum file
	QObject::connect(&model, SIGNAL(savedChecksumFile()), &app, SLOT(closeAllWindows()));

	// Progress update
	QObject::connect(&controller, SIGNAL(currentRecordProgress(int)), window->controls->file_progress, SLOT(setValue(int)));
	QObject::connect(&controller, SIGNAL(currentRecordMaxProgress(int)), window->controls->file_progress, SLOT(setMaximum(int)));
	QObject::connect(&controller, SIGNAL(currentlyProcessingFilename(QString)), window->controls->file_progress, SLOT(setText(QString)));

	QObject::connect(&controller, SIGNAL(currentlyProcessingRecord(int)), window->controls->action_progress, SLOT(setValue(int)));

	QObject::connect(window->controls->tasks, SIGNAL(filesSelected(QStringList&)), &model, SLOT(loadUrls(QStringList&)));

	QObject::connect(&controller, SIGNAL(cannotWriteChecksumFile(QString, QString)), window, SLOT(showChecksumFileWriteError(QString, QString)));

	// Parse arguments:
	QStringList args = app.arguments();
	args.removeAt(0); // First argument is the command
	if (args.size() > 0)
		model.loadUrls(args);

	// Style
// 	app.setStyleSheet("QLabel { padding: 3px; border: 1px solid black; font-weight: bold; border-radius: 4px; text-align: center}");

	app.exec();
	return 0;
}
