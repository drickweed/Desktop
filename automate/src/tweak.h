#pragma once

#include <string>
#include <vector>

#include "tweakaction.h"

/**
	A tweak.

	A tweak is an action which modified one or more settings (using a script, registry file, executable).
	Not all tweaks can be removed.

*/
class Tweak {

	private:

		std::string m_description; /// Description
		std::vector<TweakAction> m_actions; /// Tweak actions

	public:

		/**
			Default constructor.

			@param path Path to tweak file.
			@param description Description of tweak's effect.
		*/
		Tweak(const std::string description="");

		std::string description() const { return m_description; }

		void setDescription(std::string description) { m_description = description; }

		void addAction(TweakAction action) { m_actions.push_back(action); }

		std::vector<TweakAction> actions()  const { return m_actions; }
};
