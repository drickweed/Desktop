#pragma once

#include <string>

class TweakAction {

	public:
		enum TweakActionType {UNKNOWN, REGISTRY, VBSCRIPT, EXECUTABLE};

		TweakAction(std::string path, std::string description="");

		std::string description() const { return m_description; }

		std::string path() const { return m_path; }

		void setPath(std::string path) { m_path = path; }

		TweakActionType type() const;

	private:
		std::string m_description,	/// Description
						m_path;		/// Path to action file
};
