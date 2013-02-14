#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include "urls.h"

void VersionURLS::load(const std::string filename) {
	boost::filesystem::path file(filename); /// URLS file handler.
	std::string line; /// Line in urls file.
	std::vector<std::string> parts; /// Version and urls parts of the line.

	if (boost::filesystem::exists(file)) {
		std::ifstream in(file.string().c_str());
		while (getline(in, line, '\n')) {
			boost::algorithm::split(parts, line, boost::is_any_of(" "));
			m_urls[parts[0]] = parts[1];
		}
		in.close();
	}
}

std::string VersionURLS::url(std::string version) {
	if (m_urls.count(version)) {
		return m_urls[version];
	}
	return "";
}

std::vector<std::string> VersionURLS::versions() {
	std::vector<std::string> versions;
	std::map<std::string, std::string>::iterator it;

	for (it=m_urls.begin() ; it != m_urls.end(); it++) {
		versions.push_back(it->first);
	}

	return versions;
}
