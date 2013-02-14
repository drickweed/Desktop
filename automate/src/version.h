#pragma once

#include <string>
#include <exception>

/**
	A single version of an Application.
	Can be installed with Installer.
*/
class Version {

	private:

		std::string m_name,		/// Application name.
					m_version,	/// Application version.
					m_path,		/// Path to version.
					m_installer,/// Path to installer file.
					m_params,	/// Parameters to be passed to installer.
					m_url;		/// URL to download installer from.
		bool m_silent;			/// Version has silent install method.

		/**
			Load settings.
		*/
		void loadSettings();

	public:

		enum InstallerType {NONE, MSI, EXE};
		Version() { }
		/**
			Usable constructor.
			@param path Path to version's folder.
		*/
		Version(const std::string path, const std::string url="");

		/**
			Find installer on disk.
		*/
		void findInstallerFile();

		/**
			Path to version's folder.
			@return path to folder.
		*/
		std::string path() const { return m_path; }

		bool silent() const { return m_silent; }

		/**
			Application's name.
			@return name.
		*/
		std::string name() const { return m_name; }

		/**
			Application's version.
			@return version.
		*/
		std::string version() const { return m_version; }

		/**
			Installer path.
			@returns Path to installer.
		*/
		std::string installer() const { return m_installer; }

		bool hasInstaller() const { return type != NONE; }

		/**
			Installer parameters for current version.
			@return Parameters.
		*/
		std::string parameters() const { return m_params; }

		/**
			Installer download URL.
			@return URL.
		*/
		std::string url() const { return m_url; }

		bool hasUrl() const { return m_url.size() != 0; }

		/**
			Type of the installer for given Version.
		*/
		InstallerType type;

		void setUrl(std::string url) { m_url = url; }

		bool is_last; /// Latest version.

		/**
			Reload version information from database.
		*/
		void reload() { try {  findInstallerFile(); } catch (NoInstaller &e) {} }

		/**
			Exception. No installer file found.
		*/
		class NoInstaller : public std::exception {
		};

		/**
			Exception. Invalid version folder (Example: ".svn").
		*/
		class InvalidPath : public std::exception {
		};

};
