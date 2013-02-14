#pragma once

/** STL headers **/
#include <exception>

/** QT headers **/
#include <QString>

using namespace std;

class ErrorOpeningLogFile : public exception {

	public:
		ErrorOpeningLogFile(QString path):
				path(path) {
		}

		~ErrorOpeningLogFile() throw() {}

		virtual const char* what() const throw() {
			return path.toStdString().c_str();
		}

	private:
		QString path;

};

class ErrorCreatingLogPath : public exception {};

class ErrorCreatingDestinationPath : public exception {};

class DestinationPathMissing : public exception {};

class ErrorInvalidSource : public exception {};

class DestinationPathNotSet : public exception {};

class ErrorMountingNetworkPaths : public exception {};

class DestinationDeviceMissing : public exception {};

class ErrorLocatingRemovableDevice : public exception {};
