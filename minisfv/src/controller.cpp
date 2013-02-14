/** QT headers **/
#include <QDebug>
#include <QSettings>

/** Application headers **/
#include "controller.h"
#include <filechecksummodelproxy.h>

/** JB headers **/
#include <jb/std/memory.h>

namespace MiniSFV {

	Controller::Controller(QObject *parent):
		QObject(parent),
		m_busy(false),
		m_processing(false),
		index(-1),
		checksum(NULL),
		thread(NULL) {

		connect(this, SIGNAL(done()), SIGNAL(finished()));
		connect(this, SIGNAL(done()), SLOT(resultStatus()));
	}

	Controller::~Controller() {
		cleanUp();
	}

	void Controller::registerInExplorer() {

	}

	void Controller::unregisterFromExplorer() {
	}

	void Controller::startAction() {

		// Are we busy ?
		if (busy())
			return;
		// Do we have anything to act on ?
		if (lastRecordReached())
			return;
		// Set busy
		setBusy(true);
		// Start processing current record
		process();
	}

	void Controller::cleanUp() {
		if (thread) {
			thread->wait();
			thread->deleteLater();
			thread = NULL;
		}
		DELETE(checksum);
	}

	void Controller::processNext() {
		if (busy()) {
			// Move to next record
			setIndex(index + 1);
			// Finished processing last record ?
			if (lastRecordReached()) {
				setBusy(false);
				emit done();
				return;
			}
			process();
		} else
			emit done();
	}

	void Controller::process() {
		// Are we already processing ?
		if (processing())
			return; // Should never reach this point

		setProcessing(true);

		const FileChecksumRecord record = model->getRecord(index);

		// Skip validated records
		if (record.isValid()) {
			setProcessing(false);
			return;
		}

		model->unsetCalculatedChecksum(index);

		// before starting a new thread
		cleanUp();

		//TODO add different checksum generators via plugins
		// Create checksum generator
		checksum = new CRC32Checksum(record.path());

		//FIXME are connections severed when the checksum object is deleted ?
		// Connect checksum generator to controller
		connect(checksum, SIGNAL(bytesRead(int)), SIGNAL(currentRecordProgress(int)));
		connect(checksum, SIGNAL(fileSize(int)), SIGNAL(currentRecordMaxProgress(int)));
		connect(checksum, SIGNAL(checksumResult(unsigned long)), SLOT(finishProcess(unsigned long)));
		connect(this, SIGNAL(abortProcess()), checksum, SLOT(abort()));
		connect(checksum, SIGNAL(aborted()), SLOT(abortedChecksum()));

		// Checksum calculation thread
		thread = new ChecksumThread(checksum, this);
		thread->start();
	}

	void Controller::finishProcess(unsigned long checksum) {
		if (index > -1)
			model->setChecksum(index, checksum);
		setProcessing(false);
	}

	void Controller::abort() {
		if (busy())
			setBusy(false);
		if (processing())
			emit abortProcess();
	}

	void Controller::abortedChecksum() {
		setProcessing(false);
		if (not busy()) {
			emit aborted();
		}
	}

	void Controller::setModel(FileChecksumModel *model) {
		this->model = model;
	}

	void Controller::setIndex(int index) {
		this->index = index;
		emit currentlyProcessingRecord(index);

		//FIXME not using QList<T>.value(int t) because records do not (yet) have default constructors.
		if (index >= 0 and index < model->rowCount()) {
			emit currentlyProcessingFilename(model->getRecord(index).path());
		} else {
			emit currentlyProcessingFilename(QString(""));
		}
	}

	void Controller::skipCurrent() {
		if (processing()) {
			emit abortProcess();
		}
	}

	void Controller::clearActionConnections() {
		disconnect(this, SIGNAL(done()), model, SLOT(saveChecksums()));
		disconnect(this, SIGNAL(finishedProcessingSingleRecord()), this, SLOT(processNext()));
		disconnect(this, SIGNAL(finishedProcessingSingleRecord()), this, SLOT(processingFinished()));
	}

	void Controller::create(QString path) {

		model->setChecksumDatabaseFile(path);

		QFile::FileError result = model->checksumFileCanBeWritten();
		QString error_message;
		switch (result) {
			case QFile::PermissionsError:
				error_message = tr("Permission error");
				break;
			case QFile::OpenError:
				error_message = tr("Error opening file for writing");
				break;
			default:
				break;
		}

		if (result != QFile::NoError) {
			emit cannotWriteChecksumFile(model->checksumFileName(), error_message);
			return;
		}

		clearActionConnections();

		// Connect finishing calculations to save checksums file.
		connect(this, SIGNAL(done()), model, SLOT(saveChecksums()));
		// Continue processing the next file
		connect(this, SIGNAL(finishedProcessingSingleRecord()), SLOT(processNext()));
		// Start checking from beginning of list
		setIndex(0);
		startAction();
	}

	void Controller::verify() {
		clearActionConnections();
		// Continue processing the next file
		connect(this, SIGNAL(finishedProcessingSingleRecord()), SLOT(processNext()));
		// Start checking from beginning of list
		setIndex(0);
		startAction();
	}

	void Controller::verify(const QModelIndex &index) {
		clearActionConnections();
		// Process a single file
		connect(this, SIGNAL(finishedProcessingSingleRecord()), SLOT(processingFinished()));
		// Set which file to check

		model->unsetCalculatedChecksum(index.row());

		setIndex(index.row());
		startAction();
	}

	bool Controller::lastRecordReached() {
		return (index == model->rowCount());
	}

	void Controller::setProcessing(bool value) {
		m_processing = value;
		if (not value) // Signal if no longer processing
			emit finishedProcessingSingleRecord();
	}

	void Controller::setBusy(bool value) {
		m_busy = value;
		if (value) {
			emit startedAction(model->rowCount());
		} else {
			setIndex(-1); // Set index to invalid value.
		}
	}

	void Controller::resultStatus() {
		if (model->missingCount() or model->invalidCount() or model->uncheckedCount())
			emit errorsFound();
		else
			emit allValid();
	}

	void Controller::processingFinished() {
		setBusy(false);
		emit done();
	}
}
