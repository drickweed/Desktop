#pragma once

/** QT headers **/
#include <QObject>
#include <QDebug>

/** Application headers **/
#include "checksumthread.h"
#include "crc32checksum.h"
#include "filechecksummodel.h"
#include "filechecksumrecord.h"

#include <assert.h>

namespace MiniSFV {

	/** Controller

		action: top level task (eg: create checksum file, verify file list etc.)
		process: minor task (eg: calculate checksum for specific file)

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Controller : public QObject {
		Q_OBJECT
		public:
			/**
				Constructor.

				@param parent parent
			*/
			Controller(QObject *parent = 0);

			/**
				Destructor.
			*/
			~Controller();

			/**
				Set model.

				@param model model
			*/
			void setModel(FileChecksumModel *model);

			/**
				Disconnect all actions taken when processing finished.

				Run this before connecting new actions.
			*/
			void clearActionConnections();

			/**
				Set busy flag.

				NOTE: setting to true does not abort processing, only stops another processs from starting. Actual end of action is announced by done() or aborted() signals
			*/
			void setBusy(bool value);

			/**
				Get busy status.

				@return Busy status
			*/
			bool busy() const { return m_busy; }

		public slots:

			void abort(); /// Abort current action
			void abortedChecksum(); /// Process aborted
			void skipCurrent(); /// Skip current process
			void verify(const QModelIndex &index); /// Start verifying single file
			void verify(); /// Start verifying all files
			/** Start creating checksum list file

				@param path filename to save checksum list to
			*/
			void create(QString path);

			void setProcessing(bool value);

			bool processing() const {
				return m_processing;
			}

			void registerInExplorer();
			void unregisterFromExplorer();

		protected slots:
			void process(); /// Process current file (current file id is in 'index' attribute)

			void processNext(); /// Process next file (current file id is in 'index' attribute)

			/**
				Act on process results and finish processing

				@param checksum checksum value
			*/
			void finishProcess(unsigned long checksum);

		signals:
			void aborted(); /// Process aborted
			void done(); /// Action finished (emit only after process finished, not when setting busy to false).
			void abortProcess(); /// Request aborting current process
			void startedAction(int count); /// Report start of action, processing 'count' number of files

			void currentRecordProgress(int progress); /// Progress of current process
			void currentRecordMaxProgress(int value); /// Maximum progress value for current process

			void finishedProcessingSingleRecord(); /// Checksum for current record (aborted or finished)
			void currentlyProcessingRecord(int index); /// Notify currently processed index
			void finished();

			void errorsFound();
			void allValid();
			void currentlyProcessingFilename(QString path);

			void cannotWriteChecksumFile(QString filename, QString reason);

			void registeredInExplorer();
			void unregisteredFromExplorer();

		private:
			bool m_busy, /// Busy with an action
				 m_processing; /// Running a process

			/**
				Index of currently processed record
				-1 if not processing anything.
				Need to remember this because checksum processor doesn't care.
			*/
			int index;

			/**
				Set index of currently processed record

				@param index currently processed record index
			*/
			void setIndex(int index);

			/** Process clean up code

				Memory management before starting new process.
			*/
			void cleanUp();

			void startAction(); /// Start action

			/**
				Reached the last record to check.
			*/
			bool lastRecordReached();

			CRC32Checksum *checksum; /// Current checksum calculation
			ChecksumThread *thread; /// Current checksum calculation thread
			FileChecksumModel *model; /// File checksum list

		private slots:
			void resultStatus();

			/** Set "not busy" and signal that processing is done.
			*/
			void processingFinished();
	};
}

