#include "tweakindexhandler.h"

#include <boost/filesystem.hpp>

#include "utils.h"
#include "application.h"
#include "config.h"

TweakIndexHandler::TweakIndexHandler():
	inTweak(false),
	tweak(NULL) {
	element_lookup["action"] = Action;
	element_lookup["description"] = Description;
}

bool TweakIndexHandler::startElement (const QString &, const QString &, const QString &qName, const QXmlAttributes &atts ) {

	// Start processing tweak
	if (qName == "tweak") {
		tweak = new Tweak();
		inTweak = true;
	}

	// Not processing a tweak. nothing to do
	if (!inTweak)
		return true;

	m_data = "";
	//save sttributes to a map
	m_attributes.clear();
	for (int i = 0; i < atts.count(); ++i)
		m_attributes[atts.qName(i)] = atts.value(i);
	return true;
}

bool TweakIndexHandler::endElement (const QString &, const QString &, const QString &qName) {

	TweakAction *action; /// Currently parsed tweak action

	// Not in a tweak. nothing to do.
	if (!inTweak)
		return true;

	// Finished parsing tweak. put it in the tweak list.
	if (qName == "tweak") {
		inTweak = false;
		m_tweaks.push_back(*tweak);
		return true;
	}

	if (qName == "clean") // Skip clean tag. for internal use to make tweak as cleaned up from original source
		return true;

	switch (element_lookup[qName]) {
		case Action:
			action = new TweakAction(m_attributes["link"].toStdString(), m_data.toStdString());
			tweak->addAction(*action);
			break;
		case Description:
			tweak->setDescription(m_data.simplified().toStdString());
			break;
		default:
			break;
		}
	return true;
}

bool TweakIndexHandler::characters (const QString &ch ) {
	m_data += ch;
	return true;
}
