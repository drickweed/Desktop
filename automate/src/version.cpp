#include <fstream>

#include <boost/filesystem.hpp>

#include <jb/std/log.h>

#include "version.h"
#include "utils.h"

namespace fs = boost::filesystem;

Version::Version(const std::string version_path, const std::string url):
	m_path(version_path),
	m_installer(""),
	m_params(""),
	m_url(url),
	m_silent(true),
	type(NONE),
	is_last(false) {

	fs::path pathfd(path());

	if (pathfd.filename() == ".svn") {
		throw InvalidPath();
	}

	m_version = pathfd.filename();
	m_name = pathfd.parent_path().filename();

	try {
		findInstallerFile();
	} catch (NoInstaller &e) {} // Ignore missing installer file.
	loadSettings();
}

void Version::findInstallerFile() {
	fs::path pathfd(path());

	if (!fs::exists(pathfd)) {
		throw NoInstaller();
	}

	fs::directory_iterator itr(pathfd), end_itr;
	int exe_count = 0;
	while (itr != end_itr) {
		// Check if msi file exists
		if (itr->path().extension() == ".msi") {
			m_installer = itr->path().string();
			type = MSI;
			break;
		} else if (itr->path().extension() == ".exe") {
			if (exe_count == 0) { // Found single executable. use that as installer
				m_installer = itr->path().string();
// 				LOG ("Found executable installer: " << m_installer)
				type = EXE;
				exe_count++;
			} else { // Found more than one executable. don't know which one to use. must rely on other info (install.<version> file)
				type = NONE;
				exe_count++;
			}
		}
		++itr;
	}
	// Check if setup.exe exists
	if (!hasInstaller() && fs::exists(pathfd / "setup.exe")) {
		m_installer = (pathfd / "setup.exe" ).string();
		type = EXE;
	// Check for installer.<version> file to read install file from
	} else if (!hasInstaller()) {
		fs::path alt_inst = (pathfd.parent_path()) / (std::string("installer.") + version());
		if (fs::exists(alt_inst)) {
			std::ifstream in(alt_inst.string().c_str());
			char data[1024];
			while (!in.eof()) {
				in.read(data, 1024);
				m_installer.append(data);
			}
			in.close();
			m_installer = (pathfd / m_installer ).string();
			fs::path p(m_installer);
			if(p.extension() == ".msi")
				type = MSI;
			else if(p.extension() == ".exe")
				type = EXE;
		}
	}
	if (!hasInstaller())
		throw NoInstaller();
	else {
		// Convert path to native format
		m_installer = fs::path(m_installer).file_string();
	}
}

void Version::loadSettings() {
	fs::path app_path, setting_file;

	app_path = fs::path(path()).parent_path();
	// Try version specific file
	setting_file = app_path / (std::string("parameters.") + version());
	if (!fs::exists(setting_file)) {
		// Try application parameter file
		setting_file = app_path / "parameters";
	}

	if (fs::exists(setting_file)) {
		m_params = stringFromFile(setting_file.string());
	}

	setting_file = app_path / std::string("nosilent");
	m_silent = !fs::exists(setting_file);
}
