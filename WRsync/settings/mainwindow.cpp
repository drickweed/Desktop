#include "mainwindow.h"

/** QT headers **/
#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>

/** JB headers **/
#include <jb/widgets/widget.h>

/** Application headers **/
#include <wrsync/settings.h>
#include <wrsync/pluginmanager.h>
#include <wrsync/logfile.h>

using namespace JB;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	file_map_dialog(NULL) {
	Ui::MainWindow::setupUi(this);
	setupModel();
	setupUi();
	loadSettingsToUi();
	connectUi();
}

void MainWindow::changeEvent(QEvent *e) {
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::setupModel() {
	reportPlugins = new ReporterPluginsModel(this);
	reportPlugins->setPlugins(PluginManager::reporterPlugins());

	processorPlugins = new ProcessorPluginsModel(this);
	processorPlugins->setPlugins(PluginManager::processorPlugins());

	schedule = new ScheduleModel(this);

	tab_names = new QStringListModel(this);
	QStringList tab_labels;
	for (int i=0; i<tabWidget->count(); i++) {
		tab_labels.append(tabWidget->tabText(i));
	}
	tab_names->setStringList(tab_labels);

	daily_schedule = new DailyScheduleModel(this);
	daily_schedule->setSourceModel(schedule);
}

void MainWindow::unschedule() {
	QModelIndex index = dailyScheduleView->selectedRow();

	if (not index.isValid())
		return;

	schedule->removeAt(daily_schedule->mapToSource(index));
}

void MainWindow::setupUi() {
	LAPP_ << "Creating settings editor window";

	sources_editor = new FileListEditor(this);
	QBoxLayout *blayout = qobject_cast<QBoxLayout*>(sourcesTab->layout());
	blayout->insertWidget(0, sources_editor);

	reportsListView = new PluginsListView(this);
	reportsListView->setMaximumWidth(160);
	reportsListView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
	blayout = qobject_cast<QBoxLayout*>(reportsTab->layout());
	blayout->insertWidget(0, reportsListView);

	reportsListView->setModel(reportPlugins);

	reportConfigLayout = qobject_cast<QVBoxLayout*>(reporterGroupBox->layout());
	processorConfigLayout = qobject_cast<QVBoxLayout*>(destinationTab->layout());

	processorComboBox->setModel(processorPlugins);

	blayout = qobject_cast<QBoxLayout*>(scheduleTab->layout());

	calendar_controls = new Calendar::Controls();
	blayout->insertWidget(0, calendar_controls);
	calendar_controls->setDate(QDate::currentDate());

	schedule_view = new ScheduleView(this);

	blayout->insertWidget(1, schedule_view);
	schedule_view->setModel(schedule);

	tabWidget->setCurrentIndex(0);
	tabsView->setModel(tab_names);
	tabsView->hide();

	dailyScheduleView = new DailyScheduleView(this);
	blayout->insertWidget(2, dailyScheduleView);
	dailyScheduleView->setModel(daily_schedule);
}

void MainWindow::connectUi() {
	// Check sources list
	connect(validateSourcesButton, SIGNAL(clicked()), sources_editor, SLOT(validateFiles()));

	// Load plugin configuration user interface
	connect(reportsListView, SIGNAL(rowSelected(QModelIndex)), SLOT(setReportPluginConfigUi(QModelIndex)));
	connect(processorComboBox, SIGNAL(currentIndexChanged(int)), SLOT(setProcessPluginConfigUi(int)));

	// Add local path as source
	connect(addButton, SIGNAL(clicked()), SLOT(showAddFolderDialog()));

	connect(schedule_view, SIGNAL(dateSelected(QDate)), SLOT(loadDailySchedule(QDate)));
	connect(removeScheduleButton, SIGNAL(clicked()), SLOT(unschedule()));

	connect(calendar_controls, SIGNAL(dateSelected(int, int)), schedule, SLOT(setDate(int, int)));
	connect(generateFileReportButton, SIGNAL(clicked()), SLOT(generateFileReport()));
}

void MainWindow::loadSettingsToUi() {
	LAPP_ << "Loading general settings";

	Settings settings;
	sources_editor->setFileNames(settings.sources());
	reportPlugins->loadSettings(&settings);

	schedule->readSchedules(settings.schedules());

	LDEBUG_ << "Current processor UUID " << QSTR(settings.processorPlugin());

	int processor_plugin_index = processorPlugins->indexOf(settings.processorPlugin());

	if (processor_plugin_index < 0)
			processor_plugin_index = 0;

	processorComboBox->setCurrentIndex(processor_plugin_index);

	setProcessPluginConfigUi(processorComboBox->currentIndex());

	copyOpenFilesCheckBox->setChecked(settings.copyOpenFiles());
}

void MainWindow::saveSettingsFromUi() {
	LAPP_ << "Saving general settings";

	Settings settings;

	reportPlugins->saveSettings(&settings);
	processorPlugins->savePluginSettings();
	reportPlugins->savePluginSettings();

	int processor_plugin_index = processorComboBox->currentIndex();
	QString plugin_id = processorPlugins->processor(processor_plugin_index)->id();

	settings.setSources(sources_editor->fileNames());
	settings.setSchedules(schedule->writeSchedules());
	settings.setProcessorPlugin(plugin_id);
	settings.setCopyOpenFiles(copyOpenFilesCheckBox->isChecked());
	settings.sync();
}

void MainWindow::loadDailySchedule(QDate date) {
	daily_schedule->setDateFilter(date);
}

void MainWindow::closeEvent(QCloseEvent* /* event */) {
	saveSettingsFromUi();
}

void MainWindow::showAddFolderDialog() {
	QString directory = QFileDialog::getExistingDirectory(this, tr("Select folder to backup"), "");

	if (directory.isEmpty())
		return;

	sources_editor->addFile(directory);
}

void MainWindow::setReportPluginConfigUi(QWidget *config_ui) {
	QLayoutItem *item = reportConfigLayout->takeAt(0);

	if (item && item->widget()) {
		item->widget()->hide();
	}

	reportConfigLayout->insertWidget(0, config_ui);
	config_ui->show();
}

void MainWindow::setReportPluginConfigUi(QModelIndex index) {
	int row = index.row();
	setReportPluginConfigUi(reportPlugins->configUi(row));
	reporterGroupBox->setTitle(reportPlugins->name(row));
}

void MainWindow::setProcessPluginConfigUi(QWidget *config_ui) {
	QLayoutItem *item = processorConfigLayout->takeAt(1);

	if (item && item->widget()) {
		item->widget()->hide();
	}

	processorConfigLayout->insertWidget(1, config_ui);
	config_ui->show();
}

void MainWindow::setProcessPluginConfigUi(QModelIndex index) {
	int row = index.row();
	setProcessPluginConfigUi(processorPlugins->configUi(row));
}

void MainWindow::generateFileReport() {
	if (not file_map_dialog) {
		file_map_dialog = new FileMapDialog();
	}
	file_map_dialog->show();
	file_map_dialog->setSources(sources_editor->fileNames());
	file_map_dialog->refresh();
}
