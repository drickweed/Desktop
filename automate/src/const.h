#pragma once

#include <string>

const std::string LOG_FILENAME = "auto-mate.log"; /// Installation log file.

const std::string CONFIG_FILENAME = "auto-mate.ini"; /// Configuration file.

const std::string VERSION = "0.3-SVN"; /// Version.

const int status_message_delay = 3000; /// Duration (in miliseconds) messages are displayed in status bar.

/**
	Batch processing errors.
*/
namespace ProcessingError {
	enum ProcessingErrorType {None, Download, Install, Any};
}

/**
	Silent installer order.

	First : run silent installations first.
	Last : run silent installations last.
*/
namespace SilentOrder {
	enum SilentOrderTypes {First, Last};
}
