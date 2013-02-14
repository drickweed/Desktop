#pragma once

/** QT headers **/
#include <QString>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** Application settings **/
#include "settings.h"

using namespace JB;

class Target {
	public:
		Target();
		Target(QString path);

		QString path() const { return m_path; }
		void setPath(QString path) { m_path = path; }

		QString name() const;

		QString fullPath(QString name) {
			return Path::join(m_path, name);
		}

		void removeAll() const {}

	private:
		QString m_path;
};
