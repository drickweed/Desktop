#include "tweakrunner.h"

#include <boost/filesystem.hpp>

#include "config.h"
#include <jb/std/log.h>

namespace fs = boost::filesystem;

void TweakRunner::apply(TweakAction *tweak) {
	int result;
	std::string tweak_path = (fs::path(CONFIG.database()) / "tweaks" / "files" / tweak->path()).string();
	std::string command;

	switch (tweak->type()) {
		case TweakAction::REGISTRY:
			// silent import registry file : regedit /S <filename >
			command = std::string("regedit /S ") + tweak_path;
			break;
		case TweakAction::VBSCRIPT:
			// run vmscript : cscript <filename>
			command = std::string("cscript ") + tweak_path;
			break;
		case TweakAction::EXECUTABLE:
			command = tweak_path;
		default:
			return;
	}

	LOG("Running: " << command);
	result = system(command.c_str());
	LOG("Result: " << result);
}
