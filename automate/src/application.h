#pragma once

/** STL headers **/
#include <vector>
#include <map>
#include <string>
#include <exception>

/** STL headers **/
#include "version.h"

/**
	Information about an application.

	Has:
	- name : generic name displayed in list.
	- path : location of all related files.
	- versions
	- urls : one per version for downloading (http/ftp)
	- description : multi line description.
	- summary : one line summary.
*/
class Application {

	private:

		std::string m_name, 						/// Name.
					m_path; 					/// Path
		std::map<std::string, Version> m_versions; 	/// Versions
		std::string m_description,					/// Description
					m_summary; 					/// Description summary

		/**
			Reload list of available versions.

			Clears and re-populares versions from database on disk.
			Versions can be found either by examining the subdirectories under the application folder, or the urls file.
		*/
		void refreshVersions();

		/**
			Remove all version information.
		*/
		void clearVersions();

		/**
			Load description.
		*/
		void loadDescription();

	public:

		/**
			Constructor for creating useable objects.
			@param path Path to application folder.
		*/
		Application(const std::string path);

		/**
			Default deconstructor.
		*/
		~Application() { }

		/**
			Application name.

			@return Application name.
		*/
		std::string name() const { return m_name; }

		/**
			Application path.

			@return Full absolute path to application folder.
		*/
		std::string path() const { return m_path; }

		/**
			List of all versions for this application.

			@return Vector of version objects.
		*/
		std::vector<Version> versions();
		int versionCount() { return versions().size(); }

		/**
			Application description

			@return Application description.
		*/
		std::string description() const { return m_description; }

		/**
			Single line summary of application.

			@return Single line summary of application.
		*/
		std::string summary() const { return m_summary; }

		/**
			Last available version of application.

			@return Last available version.
		*/
		Version lastVersion();

		/**
			Exception. Base class for application object errors.
		*/
		class Error : public std::exception {
		};

		/**
			Exception. Application path not found in database.
		*/
		class AppNotFound : public Error {
			const char* what() const throw() { return "Application path not found in database"; }
		};

		/**
			Exception. Application has no versions defined (either as directories or download urls).
		*/
		class NoVersions : public Error {
			const char* what() const throw() { return "No versions defined for application"; }
		};

		/**
			Exception. Invalid path given for application. Certain paths cannot be used for application locations (Example: .svn).
		*/
		class InvalidPath : public Error {
			const char* what() const throw() { return "Invalid path given for application"; }
		};
};
