#include "importhandler.h"

#include <boost/filesystem.hpp>

#include "utils.h"
#include "application.h"
#include "config.h"

ImportHandler::ImportHandler() {
	element_lookup["install"] = Install;
	element_lookup["run"] = Run;
	element_lookup["include"] = Include;
	element_lookup["command"] = Command;
	element_lookup["service"] = Service;
	element_lookup["setting"] = Setting;
	element_lookup["update"] = Update;
	element_lookup["registry"] = Registry;
}

bool ImportHandler::startElement (const QString &, const QString &, const QString &qName, const QXmlAttributes &atts ) {
	m_data = "";

	if (qName == "template")
		inTemplate = true;

	if (! inTemplate)
		return true;

	//save sttributes to a map
	m_attributes.clear();
	for (int i = 0; i < atts.count(); ++i)
		m_attributes[atts.qName(i)] = atts.value(i);
	return true;
}

bool ImportHandler::endElement (const QString &, const QString &, const QString &qName) {

	if (!inTemplate)
		return true;

	if (qName == "template")
		inTemplate = false;

	switch (element_lookup[qName]) {
		case Install:
			try {
				Application app((boost::filesystem::path(CONFIG.database())/"applications"/m_data.toStdString()).string());
				// Application version attribute specified
				if (m_attributes.count("version") > 0) {
					std::vector<Version> vers = app.versions();
					// Look for application version to install
					for (std::vector<Version>::iterator itr = vers.begin(); itr != vers.end(); ++itr) {
						if (itr->version() == m_attributes["version"].toStdString()) {
							m_application_list.push_back(*itr);
							break;
						}
					}
				} else {
					Version ver = app.lastVersion();
					ver.is_last = true;
					m_application_list.push_back(ver);
				}
			} catch (Application::Error) { /* ignore unknown application */ }
			break;
		case Run:
			break;
		case Include:
			break;
		case Command:
			break;
		case Service:
			break;
		case Setting:
			break;
		case Update:
			break;
		case Registry:
			break;
		default:
			break;
		}
	return true;
}

bool ImportHandler::characters (const QString &ch ) {
	if (inTemplate)
		m_data += ch;
	return true;
}
