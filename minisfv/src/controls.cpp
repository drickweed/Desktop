#include "controls.h"

/** QT headers **/
#include <QDebug>

/** JB headers **/
#include <jb/std/memory.h>

using namespace JB;

namespace MiniSFV {

	Controls::Controls(FileChecksumModel *model, QWidget *parent):
			QWidget(parent),
			model(model) {

		proxy.setSourceModel(model);
		progress_proxy.setSourceModel(model);
		progress_proxy.changeFilter(Checksum::Invalid | Checksum::Missing, true);
		progress_proxy.changeFilter(Checksum::Valid | Checksum::Unknown, false);

		file_table = new FileChecksumTable(this);
		file_table->setModel(&proxy);
		progress_table = new FileChecksumTable(this);
		progress_table->setModel(&progress_proxy);
		progress_table->setContextMenuPolicy(Qt::NoContextMenu);

		file_progress = new ProgressBar(this);
		file_progress->showLabel();

		progress_layout.addWidget(file_progress);
		progress_layout.addWidget(progress_table, 1);

		progress_widget = new QWidget(this);
		progress_widget->setLayout(&progress_layout);

		instructions = new Instructions(this);
		instructions->setInstructions("Drop a folder, some files, or a checksum file here to start working");

		content = new QStackedLayout();
		content->addWidget(instructions);
		content->addWidget(file_table);
		content->addWidget(progress_widget);
		content->setCurrentIndex(INSTRUCTIONS);

		action_progress = new ProgressBar(this);
		action_progress->hide();

		// Summary view
		summary = new Summary(model, this);

		// Controls
		tasks = new TaskUI(this);
		abort = new AbortUI(this);
		actions = new QStackedLayout();
		actions->addWidget(tasks);
		actions->addWidget(abort);

		// Main gui
		top_layout.addLayout(actions);
		top_layout.addLayout(content, 1); // 1 makes the widget expand to fit most space in the layout
		top_layout.addWidget(summary);
		top_layout.addWidget(action_progress);

		setLayout(&top_layout);

		connect(model, SIGNAL(rowsAdded(const QModelIndex&, const QModelIndex&)), file_table, SLOT(resizeColumnsToContents()));
		connect(model, SIGNAL(rowsAdded(const QModelIndex&, const QModelIndex&)), summary, SLOT(update()));

		connect(tasks, SIGNAL(create()), SLOT(createChecksumFile()));
		connect(summary, SIGNAL(displayFilter(uint)), &proxy, SLOT(setDisplayFilter(uint)));

		connect(instructions, SIGNAL(forwardDropEvent(QDropEvent*)), SLOT(showEditorFileList()));
		connect(instructions, SIGNAL(forwardDropEvent(QDropEvent*)), file_table, SLOT(forwardedDropEvent(QDropEvent*)));
	}

	void Controls::createChecksumFile() {

		if (model->empty())
			return;

		QString path = QFileDialog::getSaveFileName(this, tr("Save checksums"), model->checksumFileName(), tr("Checksum Files (*.sfv)"));
		if (not path.isEmpty()) {
			emit createChecksumFile(path);
		}
	}

	void Controls::showTasks() {
		actions->setCurrentIndex(TASKS);
	}

	void Controls::showAbort() {
		actions->setCurrentIndex(ABORT);
	}

	void Controls::showProgressFileList() {
		summary->hide();
		action_progress->show();
	}

	void Controls::showSummary() {
		summary->show();
		action_progress->hide();
	}

	void Controls::disableAdding() {
		file_table->setAcceptDrops(false);
	}

	void Controls::enableAdding() {
		file_table->setAcceptDrops(true);
	}

	void Controls::showEditorFileList() {
		content->setCurrentIndex(EDITOR);
	}

	void Controls::showProgress() {
		content->setCurrentIndex(PROGRESS);
	}

}
