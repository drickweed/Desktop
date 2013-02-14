#pragma once

#include <vector>
#include <string>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include "application.h"

/**
	Reads folder and extracts Application objects.

	@param path Path to folder.
	@return Applicatoin vector of found apps.
*/
std::vector<Application> loadApplicationDatabase(const char* path);

/**
	Load first text line from file.

	@param filename File to load from.
	@return String if loaded, else empty string.
*/
std::string stringFromFile(std::string filename);

#ifdef Q_OS_WIN32
namespace windowsUtils {
/**
	Check vista UAC state.

	Uses registry to check.

	@return UAC active state
*/

bool UACState();

/**
	Set vista UAC state.

	Sets in registry. Requires a reboot.

	@param state UAC state to set.
*/

void setUACState(bool p_state);

bool getPrivileges(HANDLE &hToken, TOKEN_PRIVILEGES &oldtp);
bool dropPrivileges(HANDLE &hToken, TOKEN_PRIVILEGES &oldtp);

}
#endif
