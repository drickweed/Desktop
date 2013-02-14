// QT includes
#include <QApplication>

// STDLIB includes
#include <string>
#include <iostream>
#include <popt.h>

// Boost includes
#include <boost/filesystem.hpp>

// JB includes
#include <jb/qt4/app.h>
#include <jb/std/log.h>

// Application includes
#include "gui.h"
#include "controller.h"
#include "const.h"
#include "config.h"

using namespace JB;

int main(int argc, char** argv) {

	Q_INIT_RESOURCE(resources);

	QApplication app(argc, argv);

	App::init(argc, argv);
	Logger::init((boost::filesystem::path(App::path()).parent_path() / LOG_FILENAME ).string());
	LOG ("Auto-Mate started")

	// command line argument storage variables
	char auto_start=0, download_only=0;
	std::string template_name;
	const char *argument;

	// popt setup
	poptContext option_context;
	char option_id;
	struct poptOption optionsTable[] = {
		{"start", 's', POPT_ARG_VAL, &auto_start, 0, "Start running template immediatly", NULL},
		{"download-only", 'd', POPT_ARG_NONE, &download_only, 'd', "Only download applications", NULL},
		{"version", 'v', POPT_ARG_NONE, NULL, 'v', "Show version", NULL},
		POPT_AUTOHELP
		POPT_TABLEEND
	};

	// popt option processing
	option_context = poptGetContext(NULL, argc, (const char**)argv, optionsTable, 0);
	while ((option_id = poptGetNextOpt(option_context)) >= 0) {
		switch (option_id) {
			case 'h':
				poptPrintUsage(option_context, stderr, 0);
				return 0;
			case 'v':
				std::cout << "Version: " << VERSION << std::endl;
				return 0;
		}
	}

	// popt error checking
	if (option_id < -1) {
		/* an error occurred during option processing */
		fprintf(stderr, "%s: %s\n",
				poptBadOption(option_context, POPT_BADOPTION_NOALIAS),
				poptStrerror(option_id));
		return 1;
	}

	argument = poptGetArg(option_context);
	if (argument) template_name = string(argument);

	poptFreeContext(option_context);

	Config::init((boost::filesystem::path(App::path()).parent_path() / CONFIG_FILENAME ).string().c_str(), &app);

	Controller *controller = new Controller();

	AutoMateUi* ui = new AutoMateUi;

// 	app.connect(&app, SIGNAL(aboutToQuit()), controller, SLOT(abort())); /// Causes segfault. find a better way.
	app.connect(&app, SIGNAL(aboutToQuit()), &CONFIG, SLOT(sync()));
	app.connect(ui, SIGNAL(install(std::vector<Version>, bool)), controller, SLOT(processApplications(std::vector<Version>, bool)));
	app.connect(ui, SIGNAL(tweak(std::vector<TweakAction>)), controller, SLOT(processTweaks(std::vector<TweakAction>)));
	app.connect(controller, SIGNAL(dataReadProgress(int, int)), ui, SLOT(updateDataReadProgress(int, int)));
	app.connect(ui, SIGNAL(abort()), controller, SLOT(abort()));
	app.connect(controller, SIGNAL(finished(bool)), ui, SLOT(resetStartAction()));
	app.connect(controller, SIGNAL(finished(bool)), ui, SLOT(setIdle()));
	app.connect(controller, SIGNAL(aborted()), ui, SLOT(setIdle()));
	app.connect(controller, SIGNAL(aborted()), ui, SLOT(resetStartAction()));
	app.connect(controller, SIGNAL(status(QString)), ui, SLOT(updateStatusMessage(QString)));
	app.connect(controller, SIGNAL(currentProcess(QString)), ui, SLOT(updateProgressName(QString)));
	app.connect(controller, SIGNAL(started()), ui, SLOT(setBusy()));
	app.connect(controller, SIGNAL(downloadSpeed(int)), ui, SLOT(updateDownloadSpeed(int)));
	app.connect(controller, SIGNAL(startedNewDownload()), ui, SLOT(startedNewDownload()));

	// Act on command line arguments
	// FIXME all these should be done via the controller
	ui->setDownloadOnly(download_only);
	if (template_name.length() > 0) {
		ui->loadTemplate(template_name);
	}

	ui->show();

	if (auto_start)
		ui->applySelected();

	return app.exec();
}
