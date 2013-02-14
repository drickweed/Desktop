#include <string>
#include <iostream>

#include <boost/filesystem.hpp>

#include <QtGlobal>


#include "utils.h"
#include <jb/std/log.h>
#include "version.h"

std::vector<Application> loadApplicationDatabase(const char* path) {
	namespace fs = boost::filesystem;
	std::vector<Application> apps;
	fs::path db(path);

	if (fs::is_directory(path)) {
		fs::directory_iterator itr(path), end_itr;
		while (itr != end_itr) {
			fs::path app = itr->path();
			if (boost::filesystem::is_directory(app)) {
				try {
					Application a(app.string());
					// Do not load applications without any version information
					if (a.versionCount() != 0)
						apps.push_back(a);
				} catch (Application::Error) {
					//do nothing
				}
			}
			++itr;
		}
	}

	return apps;
}

std::string stringFromFile(std::string filename) {
	boost::filesystem::path file(filename);
	std::string line;

	if (boost::filesystem::exists(file)) {
		std::ifstream in(file.string().c_str());
		getline(in, line, '\n');
		in.close();
	}
	return line;
}

#ifdef Q_OS_WIN32

bool windowsUtils::getPrivileges(HANDLE &hToken, TOKEN_PRIVILEGES &oldtp) {
	//HANDLE      hToken;     /* process token */
	TOKEN_PRIVILEGES tp;    /* token provileges */
	//TOKEN_PRIVILEGES oldtp;    /* old token privileges */
	DWORD    dwSize = sizeof (TOKEN_PRIVILEGES);
	LUID     luid;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
	   LOG("OpenProcessToken() failed with code " << GetLastError())
	   return false;
	}
	if (!LookupPrivilegeValue(NULL, SE_TAKE_OWNERSHIP_NAME, &luid)) {
	   LOG ("LookupPrivilege() failed with code " << GetLastError())
	   CloseHandle (hToken);
	   return false;
	}

	ZeroMemory(&tp, sizeof(tp));
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	/* Adjust Token privileges */
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),  &oldtp, &dwSize)) {
	   LOG("AdjustTokenPrivileges() failed with code " << GetLastError())
	   CloseHandle (hToken);
	   return false;
	}
	return true;
}

bool windowsUtils::dropPrivileges(HANDLE &hToken, TOKEN_PRIVILEGES &oldtp) {
	DWORD dwSize = sizeof (TOKEN_PRIVILEGES);

	AdjustTokenPrivileges (hToken, FALSE, &oldtp, dwSize, NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS) {
		LOG("AdjustTokenPrivileges() failed with code " << GetLastError())
		CloseHandle (hToken);
		return false;
	}
	CloseHandle (hToken);
	return true;
}

bool windowsUtils::UACState() {
	HKEY hkey;
	DWORD dwDisposition;
	DWORD dwType, dwSize;
	DWORD state=0;
	LONG result;
	HANDLE hToken;     /* process token */
	TOKEN_PRIVILEGES oldtp;

	if (!windowsUtils::getPrivileges(hToken, oldtp)) {
		return false;
	}

	result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, 0, 0, NULL, &hkey, &dwDisposition);
	if (result != ERROR_SUCCESS) {
		LOG("Faile opening System policy registry keys (error " << result << ")")
		CloseHandle (hToken);
		return false;
	}

	dwType = REG_DWORD;
	dwSize = sizeof(DWORD);
	RegQueryValueEx(hkey, TEXT("EnableLUA"), NULL, &dwType, (PBYTE)&state, &dwSize);
	RegCloseKey(hkey);
	LOG("UAC controls state: " << state)

	windowsUtils::dropPrivileges(hToken, oldtp);
	return state == 1;
}

void windowsUtils::setUACState(bool p_state) {
	HKEY hkey;
	DWORD dwDisposition;
	DWORD dwType, dwSize;
	DWORD state=p_state?1:0;

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, 0, 0, NULL, &hkey, &dwDisposition) != ERROR_SUCCESS) {
		LOG("Faile opening System policy registry keys")
		return;
	}

	dwType = REG_DWORD;
	dwSize = sizeof(DWORD);
	RegSetValueEx(hkey, TEXT("EnableLUA"), 0, dwType, (PBYTE)&state, dwSize);
	RegCloseKey(hkey);
}

#endif
