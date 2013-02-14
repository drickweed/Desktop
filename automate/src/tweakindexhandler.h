#pragma once

#include <QXmlDefaultHandler>
#include <map>
#include <vector>

#include "tweak.h"

/**
	Load tweak index.
*/
class TweakIndexHandler : public QXmlDefaultHandler {
	private:

		QString m_data; 							/// Character data from xml file
		std::map<QString, QString> m_attributes; 	/// Element attributes
		bool inTweak; 								/// Currently inside a tweak element
		std::vector<Tweak> m_tweaks; 				/// Tweaks
		Tweak *tweak; 								/// Currently parsed tweak

		std::map<QString, short> element_lookup; /// Names of element types.

	public:

		enum Elements {Action, Description};

		TweakIndexHandler();

		bool startElement (const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);

		bool endElement (const QString &namespaceURI, const QString &localName, const QString &qName);

		bool characters (const QString &ch);

		/**
			Get all tweaks.

			@return tweaks
		*/
		std::vector<Tweak> tweaks() const { return m_tweaks; }
};
