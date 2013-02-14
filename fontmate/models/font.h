#pragma once

/** QT headers **/
#include <QFont>

/** JB headers **/
#include <jb/database/object.h>

using namespace JB;

namespace FontMate {

	class Font : public RelationalObject {
		Q_OBJECT
		RELATIONAL_OBJECT(Font);

		SIMPLE_PROPERTY(QString, family, setFamily, m_family);
		Q_PROPERTY(QString family READ family WRITE setFamily)

		SIMPLE_PROPERTY(int, weight, setWeight, m_weight);
		Q_PROPERTY(int weight READ weight WRITE setWeight)

		SIMPLE_PROPERTY(QString, path, setPath, m_path);
		Q_PROPERTY(QString path READ path WRITE setPath)

	public:
		QString table() const { return "fonts"; }
		QString fieldName() const { return "font"; }

		static Font* fromQFont(QFont font);

	};

}
