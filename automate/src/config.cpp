#include "config.h"

#include <boost/filesystem.hpp>

#include <jb/std/log.h>
#include <jb/qt4/app.h>

using namespace JB;

// Class static members definition.
QSettings *Config::settings = NULL;
std::map<int, QString> Config::lookup;

void Config::init(QString path, QObject *parent) {
	settings = new QSettings(path, QSettings::IniFormat, parent);
	lookup[Database] = QString("Database");
	lookup[Overwrite] = QString("OverwriteDownloads");
	lookup[ShowHelp] = QString("ShowHelp");
	lookup[StopProcessingOn] = QString("StopProcessingOn");
	lookup[SilentOrder] = QString("SilentOrder");
}

std::string Config::database() const {
	return settings->value(lookup[Database], QVariant((boost::filesystem::path(App::path()).parent_path() / "database").string().c_str())).toString().toStdString();
}

void Config::setDatabase(std::string path) {
// FIXME find why signal from singleton doesn't work
// 	if (QString::compare(QString(path.c_str()), QString(database().c_str()), Qt::CaseInsensitive) != 0) {
// 		emit databaseLocationChanged();
// 		LOG("Database location changed")
// 	}
	settings->setValue(lookup[Database], path.c_str());
}

bool Config::overwrite() const {
	return settings->value(lookup[Overwrite], QVariant(false)).toBool();
}

void Config::setOverwrite(bool value) {
	settings->setValue(lookup[Overwrite], value);
}

bool Config::showHelp() const {
	return settings->value(lookup[ShowHelp], QVariant(true)).toBool();
}

void Config::setShowHelp(bool value) {
	settings->setValue(lookup[ShowHelp], value);
}

ProcessingError::ProcessingErrorType Config::stopProcessingOn() const {
	return static_cast<ProcessingError::ProcessingErrorType>(settings->value(lookup[StopProcessingOn], QVariant(ProcessingError::None)).toInt());
}

void Config::setStopProcessingOn(ProcessingError::ProcessingErrorType value) {
	settings->setValue(lookup[StopProcessingOn], static_cast<int>(value));
}

void Config::setSilent(SilentOrder::SilentOrderTypes order) {
	settings->setValue(lookup[Config::SilentOrder], order);
}

void Config::sync() {
	settings->sync();
}
