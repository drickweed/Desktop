#pragma once

/** QT headers **/
#include <QMap>
#include <QString>

/** Application headers **/
#include <wrsync/result.h>

class Messages {
	public:

		Messages();

		/**
			Translate result state to human readable message.
		*/
		static QString state(Result::State state);

		/**
			Translate rsync exit code to human readable message.
		*/
		static QString rsync(int exit_code);

		static void init_messages();

	private:
		static QMap<int, QString> m_state_messages;
		static QMap<int, QString> m_rsync_messages;
};


