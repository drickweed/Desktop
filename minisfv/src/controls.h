#pragma once

/** QT headers **/
#include <QWidget>

/** JB headers **/
#include <jb/widgets/progress.h>
#include <jb/widgets/instructions.h>

/** Application headers **/
#include "taskui.h"
#include "abortui.h"
#include "summary.h"
#include "filechecksummodel.h"
#include "filechecksummodelproxy.h"
#include "filechecksumtable.h"

using namespace JB::Widgets;
using namespace JB;

namespace MiniSFV {

	enum action_pages {TASKS, ABORT};
	enum content_pages {INSTRUCTIONS, EDITOR, PROGRESS};

	/**
		Application main controls

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Controls : public QWidget {
		Q_OBJECT

		public:
			Controls(FileChecksumModel *model, QWidget *parent = 0);

			ProgressBar *file_progress, ///> Progress bar for scanning a single file
					 *action_progress; ///> Progress bar for action (create, verify)
			Summary *summary; ///> Summary
			TaskUI *tasks; ///> Create/Verify buttons
			AbortUI *abort; ///> Abort/Skip buttons
			FileChecksumTable *progress_table,
							  *file_table;
			Instructions *instructions; /// Instructions box
			FileChecksumModelProxy proxy, progress_proxy;

		public slots:
			/**
				Create checksum file.

				NOTE Asks user for filename
			*/
			void createChecksumFile();
			void enableAdding();
			void disableAdding();

			/**
				Show Create/Verify buttons.
			*/
			void showTasks();

			/**
				Show Skip/Stop buttons.
			*/
			void showAbort();

			/**
				Show progress file list with only errors.
			*/
			void showProgressFileList();

			/**
				Show file list.
			*/
			void showEditorFileList();

			/**
				Show action progress.
			*/
			void showProgress();

			/**
				Show result summary.
			*/
			void showSummary();

		signals:
			/**
				Create checksum file.

				@param path filename
			 */
			void createChecksumFile(QString path);

		private:

			FileChecksumModel *model; /// model containing file names and checksums
			QVBoxLayout top_layout, /// top layout
						progress_layout;
			QWidget *progress_widget;
			QStackedLayout *actions, /// Actions
						   *content; /// File list and global progress

	};
}
