#pragma once

/** JB headers **/
#include <jb/std/property.h>

/** QT headers **/
#include <QString>

namespace ImageImp {
	class ExportSettings {

		SIMPLE_STATIC_PROPERTY(int, width, setWidth, m_width);
		SIMPLE_STATIC_PROPERTY(int, height, setHeight, m_height);

		public:

		private:
			ExportSettings();

	};

}
