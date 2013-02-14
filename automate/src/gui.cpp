#include <QMessageBox>
#include <QFileDialog>
#include <QXmlInputSource>
#include <QDomElement>
#include <QSortFilterProxyModel>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include <boost/filesystem.hpp>

#include <jb/std/log.h>

#include "gui.h"
#include "appsmodel.h"
#include "utils.h"
#include "importhandler.h"
#include "const.h"
#include "config.h"
#include "helpui.h"
#include "controller.h"
#include "tweak.h"

namespace fs = boost::filesystem;

AutoMateUi::AutoMateUi(QWidget* parent) :
	QMainWindow(parent),
	logWatcher(NULL),
	applications_model(NULL),
	tweaks_model(NULL),
	selectedApplicationCount(0),
	last_database_location(CONFIG.database()) {
	setupUi(this);
	setIdle();

	// Proxy models. must be ready before loading models.
	appsProxyModel = new QSortFilterProxyModel(this);
	tweaksProxyModel = new QSortFilterProxyModel(this);

	// Reload models
	reloadModels();

	// Watch log file
	logWatcher = new TextFileWatcher(Logger::path(), log_browser);
	config = new ConfigurationUi(this);

	// Rearrange toolbar order
	// file_toolBar, action_toolBar
	addToolBar(Qt::TopToolBarArea, file_toolBar);
	addToolBarBreak(Qt::TopToolBarArea);
	addToolBar(Qt::TopToolBarArea, action_toolBar);

	connectUi();
	actionsConnect();
}

void AutoMateUi::setupUi(QMainWindow *widget) {
	Ui::AutoMate::setupUi(widget);

	// Setup search bar
	// NOTE set this up BEFORE the download checkbox below. Helps preventing search box getting focus when
	// download checkbox is hidden due to switching tabs.
	searchEdit = new FancyLineEdit(widget);
	searchShortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
	file_toolBar->addSeparator();
	file_toolBar->addWidget(searchEdit);

	// Setup donwload option
	download_checkBox = new QCheckBox(widget);
	download_checkBox->setText("Download only");
	// NOTE need to save the action widget in order to hide/show it later on. cannot hide/show the actuall widget.
	// NOTE can only use setVisible(bool) to hide/show the action (See QT reference)
	download_Action = action_toolBar->addWidget(download_checkBox);

	// setup quick help
	if(!CONFIG.showHelp()) {
		help_textBrowser->hide();
		hidehelp_checkBox->hide();
	}

	// Add progress bar to status bar
	progressName = new QLabel();
	progressName->hide();
	downloadProgressBar = new QProgressBar(widget);
	downloadProgressBar->setMaximum(0);
	statusBar()->addPermanentWidget(progressName);
	statusBar()->addPermanentWidget(downloadProgressBar);
	downloadProgressBar->hide();

	// Start action changes dynamically according to visible tab
	applyActionToStartButton();

// #ifdef Q_OS_WIN32
	// // Setup vista UAC state
	// actionVista_UAC_prompt->setChecked(windowsUtils::UACState());
	// menuSettings->addAction(actionVista_UAC_prompt);
// #endif
}

#ifdef Q_OS_WIN32
void AutoMateUi::setUACState(bool state) {
	windowsUtils::setUACState(state);
	requestReboot();
}

void AutoMateUi::requestReboot() {
	QMessageBox::StandardButton result = QMessageBox::question(this, "Reboot required", "Changing this setting requires a reboot. Reboot now ?", QMessageBox::Yes | QMessageBox::No);
	if (result == QMessageBox::Yes)
		ExitWindowsEx(EWX_REBOOT, 0);
}
#endif

void AutoMateUi::connectUi() {
	connect(clear_log_button, SIGNAL(clicked()), log_browser, SLOT(clear()));
	connect(searchEdit, SIGNAL(textEdited(const QString&)), SLOT(filterViews()));
	connect(config, SIGNAL(accepted()), SLOT(reloadModelsOnDatabaseChange()));
	connect(hidehelp_checkBox, SIGNAL(toggled(bool)), SLOT(hideHelp()));
	connect(this, SIGNAL(start()), SLOT(install()));
	connect(tabs, SIGNAL(currentChanged(int)), SLOT(applyActionToStartButton()));
	connect(download_checkBox, SIGNAL(toggled(bool)), SLOT(applyActionToStartButton()));
	connect(searchShortcut, SIGNAL(activated()), searchEdit, SLOT(setFocus()));
	connect(tweaks_view, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(tweakContextMenu(const QPoint&)));
}

void AutoMateUi::tweakContextMenu(const QPoint &p) {
	QModelIndex index = tweaks_view->indexAt(p);
	if (!index.isValid())
		return;

	index = tweaksProxyModel->mapToSource(index);
	QMenu *menu = new QMenu(this);
	ModelItem *item = static_cast<ModelItem*>(index.internalPointer());
	selectedTweakFileName = item->data(1).toString();
	if (selectedTweakFileName.size() ==0)
		return;
	QAction *action = menu->addAction(QString("Edit %1").arg(selectedTweakFileName));
	connect(action, SIGNAL(triggered()), SLOT(editSelectedTweak()));

	menu->exec(tweaks_view->viewport()->mapToGlobal(p));
}

void AutoMateUi::editSelectedTweak() {
	QString path =  QString((fs::path(CONFIG.database()) / "tweaks" / "files" / selectedTweakFileName.toStdString()).string().c_str());
	std::string command = std::string("notepad ") + path.toStdString();
	LOG("Running: " << command);
	int result = system(command.c_str());
	if (result != 0)
		LOG("Error opening tweak for editing (error: " << result);
}

void AutoMateUi::showQuickHelp() {
	CONFIG.setShowHelp(true);
	hidehelp_checkBox->show();
	hidehelp_checkBox->setChecked(false);
	help_textBrowser->show();
}

void AutoMateUi::actionsConnect() {
	connect(actionAbout, SIGNAL(triggered(bool)), SLOT(about()));
	connect(actionNew, SIGNAL(triggered(bool)), SLOT(clearSelection()));
	connect(actionLoad, SIGNAL(triggered(bool)), SLOT(load()));
	connect(actionSave, SIGNAL(triggered(bool)), SLOT(save()));
	connect(actionReload_database, SIGNAL(triggered(bool)), SLOT(reloadModels()));
	connect(actionConfiguration, SIGNAL(triggered(bool)), config, SLOT(exec()));
	connect(actionShow_quick_help, SIGNAL(triggered(bool)), SLOT(showQuickHelp()));
// #ifdef Q_OS_WIN32
	// connect(actionVista_UAC_prompt, SIGNAL(triggered(bool)), this, SLOT(setUACState(bool)));
// #endif
	connect(actionContents, SIGNAL(triggered(bool)), SLOT(showContents()));
}

void AutoMateUi::hideHelp() {
	CONFIG.setShowHelp(false);
}

void AutoMateUi::applySelected() {
	emit start();
}

void AutoMateUi::quit() {
}

void AutoMateUi::about() {
	QMessageBox::about(this, QString("Auto-Mate %1").arg(VERSION.c_str()), QString("<center><b>Auto-Mate %1</b></center>\nAutomates installing applications and performing other actions. A GUI front-end for an application and settings database.").arg(VERSION.c_str()));
}

void AutoMateUi::save() {
	std::vector<Version> vers = applications_model->getSelected();
	QDomDocument doc("export");
	QDomElement root = doc.createElement("template");
	doc.appendChild(root);
	for(std::vector<Version>::iterator itr = vers.begin(); itr != vers.end(); ++itr)
	{
		QDomElement inst = doc.createElement("install");
		if (! itr->is_last)
			inst.setAttribute("version",itr->version().c_str());
		QDomText name = doc.createTextNode(itr->name().c_str());
		inst.appendChild(name);
		root.appendChild(inst);
	}
	QString flname = QFileDialog::getSaveFileName(this,"Select file to save", "", "XML files(*.xml)");
	QFile *fl = new QFile(flname);
	if (!fl->open(QIODevice::WriteOnly | QIODevice::Text)) {
		statusBar()->showMessage("Writing to file failed", status_message_delay);
		return;
	}
	fl->write(doc.toString().toLocal8Bit());
	fl->close();
	statusBar()->showMessage("Selection saved", status_message_delay);
}

//TODO move loading to controller as static method
void AutoMateUi::loadTemplate(std::string filename) {
	ImportHandler *handler = new ImportHandler;

	QFile *fh = new QFile(QString(filename.c_str()));
	if (!(fh->exists()))
		return;

	QXmlInputSource *src = new QXmlInputSource(fh);
	QXmlSimpleReader *reader = new QXmlSimpleReader;
	reader->setContentHandler(handler);
	reader->parse(src);
	std::vector<Version> versionsToInstall = handler->installList();
	selectedApplicationCount = versionsToInstall.size();
	applyActionToStartButton();
	std::vector<QModelIndex> toExpand = applications_model->setSelected(versionsToInstall);
	for (unsigned int i = 0; i < toExpand.size(); ++i) {
		apps_view->expand(toExpand[i]);
	}
	apps_view->repaint();
	LOG("Loaded template " << filename);
}

void AutoMateUi::load() {
	QString filename = QFileDialog::getOpenFileName(this, "Select a file to load", "", "XML files(*.xml)");
	if (filename.isEmpty()) {
		statusBar()->showMessage("No template selected", status_message_delay);
		return;
	}
	loadTemplate(filename.toStdString());
}

void AutoMateUi::clearSelection() {
	applications_model->clearSelection();
	apps_view->repaint();
	tweaks_model->clearSelection();
	tweaks_view->repaint();
	selectedApplicationCount = 0;
	applyActionToStartButton();
	statusBar()->showMessage("Selection cleared", status_message_delay);
}

void AutoMateUi::tweak() {
	applyAbortToStartButton();
	emit tweak(tweaks_model->getSelected());
}

void AutoMateUi::applyAbortToStartButton() {

// 	disconnect(applications_model, SIGNAL(itemChecked(void*)), this, SLOT(addToSelection(void*)));
// 	disconnect(applications_model, SIGNAL(itemUnChecked(void*)), this, SLOT(removeFromSelection(void*)));
	switch (tabs->currentIndex()) {
		case 0: // Applications
			if (download_checkBox->isChecked())
				actionApply->setText("Stop downloading");
			else
				actionApply->setText("Stop installing");
			break;
		case 1: // Tweaks
			actionApply->setText("Stop applying");
			break;
		default:
			break;
	}

// 	actionApply->setText("Stop");
	actionApply->setIcon(QIcon(QPixmap(":/icons/abort.png")));
	disconnect(actionApply, 0, this, 0);
	disconnect(tabs, SIGNAL(currentChanged(int)), this, SLOT(applyActionToStartButton()));

	// NOTE no need. already hidden when not needed
// 	disconnect(download_checkBox, SIGNAL(toggled(bool)), this, SLOT(applyActionToStartButton()));
	connect(actionApply, SIGNAL(triggered(bool)), SIGNAL(abort()));
	download_Action->setVisible(false);
	downloadProgressBar->show();
	progressName->show();
}

void AutoMateUi::updateDownloadSpeed(int kbps) {
	downloadProgressBar->setFormat(QString("%p% (%1 kbps)").arg(kbps));
}

void AutoMateUi::startedNewDownload() {
	downloadProgressBar->setFormat("%p%");
}

void AutoMateUi::install() {
	std::vector<Version> versions = applications_model->getSelected();

	// Nothing selected. do nothing.
	if (versions.empty())
			return;

	// Change the install action to "stop"
	applyAbortToStartButton();
	emit install(versions, download_checkBox->isChecked());
}

void AutoMateUi::updateDataReadProgress(int bytesRead, int totalBytes) {
	downloadProgressBar->setMaximum(totalBytes);
	downloadProgressBar->setValue(bytesRead);
}

void AutoMateUi::applyActionToStartButton() {

	QString message;
	disconnect(actionApply, 0, this, 0);

	switch (tabs->currentIndex()) {
		case 0: // Applications
			actionApply->setEnabled(selectedApplicationCount > 0);
			download_Action->setVisible(true);
			if (selectedApplicationCount > 0)
				if (download_checkBox->isChecked()) {
					actionApply->setText(QString("Download (%1 Selected)").arg(selectedApplicationCount));
				} else {
					actionApply->setText(QString("Install (%1 Selected)").arg(selectedApplicationCount));
				}
			else {
				message = QString("Select applications to %1 in the list below");
				if (download_checkBox->isChecked()) {
					actionApply->setText(message.arg("download"));
				} else {
					actionApply->setText(message.arg("install"));
				}
			}
			connect(actionApply, SIGNAL(triggered(bool)), SLOT(install()));
			break;
		case 1: // Tweaks
			actionApply->setEnabled(true);
			actionApply->setText("Apply");
			download_Action->setVisible(false);
			connect(actionApply, SIGNAL(triggered(bool)), SLOT(tweak()));
			break;
		default: // Disable button
			download_Action->setVisible(false);
			actionApply->setText("Log");
			actionApply->setEnabled(false);
			break;
	}
}

void AutoMateUi::resetStartAction() {
	actionApply->setIcon(QIcon(QPixmap(":/icons/install.png")));
	downloadProgressBar->reset();
	downloadProgressBar->hide();
	progressName->setText("");
	progressName->hide();
	// FIXME this is ugly. should only refresh changes on selected applications.
	reloadModels();
	connect(tabs, SIGNAL(currentChanged(int)), SLOT(applyActionToStartButton()));
	connect(download_checkBox, SIGNAL(toggled(bool)), SLOT(applyActionToStartButton()));
	applyActionToStartButton();
}

void AutoMateUi::updateStatusMessage(QString message) {
	statusBar()->showMessage(message, status_message_delay);
}

void AutoMateUi::filterViews() {
	static_cast<QSortFilterProxyModel*>(apps_view->model())->setFilterRegExp(QRegExp(searchEdit->text(), Qt::CaseInsensitive, QRegExp::FixedString));
	static_cast<QSortFilterProxyModel*>(tweaks_view->model())->setFilterRegExp(QRegExp(searchEdit->text(), Qt::CaseInsensitive, QRegExp::FixedString));
}

void AutoMateUi::addToSelection(void* /* item */) {
	selectedApplicationCount++;
	if (not busy())
		applyActionToStartButton();
}

void AutoMateUi::removeFromSelection(void* /* item */) {
	selectedApplicationCount--;
	if (not busy())
		applyActionToStartButton();
}

void AutoMateUi::reloadModels() {
	std::vector<Version> selection;

	// Save selection
	if (applications_model)
		selection = applications_model->getSelected();

	// Reload application database
	appsProxyModel->setSourceModel(NULL);
	DELETELATER(applications_model)
	applications_model = new ApplicationsModel(loadApplicationDatabase((boost::filesystem::path( CONFIG.database() ) / "applications").string().c_str()));

	// Replace application selection
	applications_model->setSelected(selection);
	appsProxyModel->setSourceModel(applications_model);
	connect(applications_model, SIGNAL(itemChecked(void*)), SLOT(addToSelection(void*)));
	connect(applications_model, SIGNAL(itemUnChecked(void*)), SLOT(removeFromSelection(void*)));

	// Reload tweaks database
	tweaksProxyModel->setSourceModel(NULL);
	DELETELATER(tweaks_model)
	tweaks_model = new TweaksModel(Controller::loadTweaks(), this);
	tweaksProxyModel->setSourceModel(tweaks_model);

	// Setup tweaks view
	tweaksProxyModel->setFilterKeyColumn(-1);
	tweaks_view->setModel(tweaksProxyModel);
	tweaks_view->resizeColumnToContents(0);
	tweaks_view->setColumnHidden(1, true); // Tweak filename
	tweaks_view->setColumnHidden(2, true); // Main tweak description for multiple tweak actions

	// Setup application view
	appsProxyModel->setFilterKeyColumn(-1);
	apps_view->setModel(appsProxyModel);
	apps_view->sortByColumn(0, Qt::AscendingOrder);
	apps_view->setColumnHidden(4, true); // Hide parent name for search. This must be set after the model was setup.
	apps_view->setColumnHidden(3, true); // Hide urls
}

// FIXME ugly hack. this should be elsewhere
void AutoMateUi::reloadModelsOnDatabaseChange() {
	if (QString::compare(QString(last_database_location.c_str()), QString(CONFIG.database().c_str()), Qt::CaseInsensitive) != 0) {
		last_database_location = CONFIG.database();
		reloadModels();
	}
}

void AutoMateUi::updateProgressName(QString name) {
	progressName->setText(name);
}

void AutoMateUi::showContents() {
	HelpUi help(this);
	help.exec();
}
