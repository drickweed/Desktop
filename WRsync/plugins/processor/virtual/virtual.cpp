#include "virtual.h"

/** QT headers **/
#include <QDir>
#include <QtPlugin>

/** JB headers **/
#include <jb/std/log.h>
#include <jb/qt4/stringutils.h>

using namespace JB;

Virtual::Virtual():
	QObject(),
	m_config_dialog(NULL) {
}

ProcessorInterface::RunState Virtual::run() {

}

void Virtual::setSources(QStringList sources) {

	foreach(QString source_path, sources) {
		m_sources << Source(source_path);
	}
}

QString Virtual::commandLine(Source source) const {
	QString command_line;

	return command_line;
}

void Virtual::saveConfig() {

}

void Virtual::loadConfig() {

}

QWidget* Virtual::configUi() {
	if (m_config_dialog == NULL)
		m_config_dialog = new QWidget();
	return m_config_dialog;
}

Q_EXPORT_PLUGIN2(processor_virtual, Virtual)
