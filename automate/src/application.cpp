#include "application.h"

#include <boost/filesystem.hpp>

/** STL headers **/
#include <fstream>

/** Application headers **/
#include "urls.h"

Application::Application(const std::string path_name) :
	m_path(path_name) {

	// Check if path exists
	boost::filesystem::path pathfd(path());
	if (!boost::filesystem::exists(pathfd))
		throw AppNotFound();

	m_name = pathfd.filename();

	// The following paths are special cases and must not be used for applications
	if (name() == ".svn")
		throw InvalidPath();

	refreshVersions();
	loadDescription();
}

void Application::loadDescription() {
	boost::filesystem::path pathfd(path()); /// Description file
	pathfd = pathfd / "description";
	std::string line; /// Text line
	int index = 0;

	if (boost::filesystem::exists(pathfd)) {
		std::ifstream in(pathfd.string().c_str());
		while (getline(in, line, '\n')) {
			if (index == 0) m_summary = line;
			m_description = m_description + line;
			index++;
		}
		in.close();
	}
}

void Application::clearVersions() {
	m_versions.clear();
}

void Application::refreshVersions() {
	boost::filesystem::path pathfd(path());

	// Remove all versions
	clearVersions();

	// Get versions from filesystem
	boost::filesystem::directory_iterator itr(path()), end_itr;
	while (itr != end_itr) {
		if (boost::filesystem::is_directory(itr->path())) {
			try {
				Version ver(itr->path().string());
				m_versions[ver.version()] = ver;
			} catch (std::exception &e) {}
		}
		itr++;
	}

	// Update versions from "urls" file
	VersionURLS urlLoader;
	urlLoader.load((pathfd / "urls").string());
	std::map<std::string, std::string> urls = urlLoader.urls();
	std::map<std::string, std::string>::iterator it;

	for (it=urls.begin(); it != urls.end(); it++) {
		// Existing version was found. set url for it.
		if (m_versions.find(it->first) != m_versions.end()) {
			m_versions[it->first].setUrl(it->second);
		}
		// Existing version not found. create new one.
		else {
			Version ver((pathfd / it->first).string(), it->second);
			m_versions[it->first] = ver;
		}
	}
}

std::vector<Version> Application::versions() {
	std::vector<Version> versions;
	for (std::map<std::string, Version>::iterator itr = m_versions.begin(); itr != m_versions.end(); ++itr) {
		versions.push_back(itr->second);
	}

	return versions;
}

Version Application::lastVersion() {
	if (m_versions.size() < 1)
		throw NoVersions();

	std::map<std::string, Version>::iterator itr = m_versions.begin();
	Version lastVer = itr->second;

	while (itr != m_versions.end()) {
		if (itr->second.version() > lastVer.version())
			lastVer = itr->second;
		itr++;
	}

	return lastVer;
}
