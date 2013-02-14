#include "tweakaction.h"

#include <boost/filesystem.hpp>
#include <QString>

#include <jb/std/log.h>

namespace fs = boost::filesystem;

TweakAction::TweakAction(std::string path, std::string description):
	m_description(description),
	m_path(path) {
}

TweakAction::TweakActionType TweakAction::type() const {
	fs::path pathfd(path());
	std::string extension = QString(pathfd.extension().c_str()).toLower().toStdString();

	LOG("Tweak extension " << extension);

	if (extension == ".reg") {
		return REGISTRY;
	} else if (extension == ".vbs") {
		return VBSCRIPT;
	} else if (extension == ".exe") {
		return EXECUTABLE;
	} else {
		return UNKNOWN;
	}
}
