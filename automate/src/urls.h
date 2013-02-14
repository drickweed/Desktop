#pragma once

#include <string>
#include <map>
#include <vector>

/**
	Version URL list.

	Loads urls for application versions from file.

	@author Shaar Gabriel <gabi@jbox-comp.com>
*/
class VersionURLS {

	private:
		std::map<std::string, std::string> m_urls; /// URLs for application versions.

	public:

		/**
			Get url for given version.

			Throws NoUrl if no url found for given version.

			@param version version to get url for.
			@return url for given version.
		*/
		std::string url(std::string version);

		/**
			Load urls from given file.

			File MUST contain lines of the form : version url

			@param filename File to load urls from.
		*/
		void load(const std::string filename);

		/**
			Get all versions with urls.

			@return list of versions.
		*/
		std::vector<std::string> versions();

		std::map<std::string, std::string> urls() const { return m_urls; }

};
