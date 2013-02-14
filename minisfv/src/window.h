#pragma once

/** QT headers **/
#include <QWidget>
#include <QStackedLayout>

/** Application headers **/
#include "filechecksummodel.h"
#include "filechecksummodelproxy.h"
#include "controls.h"

using namespace JB;

namespace MiniSFV {

	/**
		Main window.

		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Window : public QWidget {
		Q_OBJECT
		public:
			/**
				Constructor.

				@param model model
				@param parent Parent QObject
			*/
			Window(FileChecksumModel *model, QWidget *parent = 0);

		public slots:

			void showChecksumFileWriteError(QString filename, QString reason);

		public:
			Controls *controls; /// Main application controls

		private:
			QStackedLayout *top_stack; /// Top stack
	};
}

