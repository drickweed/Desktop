#pragma once

#include <QXmlDefaultHandler>
#include <map>
#include <vector>

#include "version.h"

/**
	Import installer template.

*/
class ImportHandler : public QXmlDefaultHandler {
	private:

		QString m_data; /// Character data from xml file
		std::map<QString, QString> m_attributes; /// Element attributes
		bool inTemplate; /// for validity
		std::vector<Version> m_application_list; /// Vector to store all versions that are found.

		std::map<QString, short> element_lookup; /// Names of element types.

	public:

		enum Elements {Install, Run, Include, Command, Service, Setting, Update, Registry};

		ImportHandler();

		bool startElement (const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);

		bool endElement (const QString &namespaceURI, const QString &localName, const QString &qName);

		bool characters (const QString &ch);

		/**
			Get all the versions to install.
			@return vector of versions to instal..
		*/
		//FIXME I highly suggest to bundle all of the outputs into some struct and ship out in single method.
		std::vector<Version> installList() const { return m_application_list; }
};
