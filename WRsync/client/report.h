#pragma once

/** QT headers **/
#include <QObject>
#include <QList>

/** Application headers **/
#include "processor.h"
#include <wrsync/result.h>
#include <wrsync/pluginmanager.h>
#include <wrsync/settings.h>
#include <plugins/interface/reporter.h>

using namespace WRsync;

class Report : public QObject {
	Q_OBJECT
	public:
		Report(Processor *processor, Settings *settings, QObject *parent=0);
		ReporterInterface* firstReporter() const;

	private:
		QList<ReporterInterface*> activeReporterPlugins() const;

		Settings *m_settings;
		Processor *m_processor;

	public slots:
		void reportUsingPlugins();

	signals:
		void done();

};

