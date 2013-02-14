#pragma once

#include "model.h"
#include "tweak.h"

class TweaksModel : public Model {
		Q_OBJECT
	public:
		TweaksModel(std::vector<Tweak> data, QObject *parent = 0);

		void clearSelection();

		std::vector<TweakAction> getSelected();
};
