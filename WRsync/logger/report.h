#pragma once

/** QT headers **/
#include <QObject>
#include <QList>

/** Application headers **/
#include <wrsync/result.h>
#include <wrsync/pluginmanager.h>
#include <wrsync/settings.h>
#include <plugins/interface/reporter.h>

using namespace WRsync;

class Report : public QObject {
	Q_OBJECT
	public:
		Report(Settings *settings, QObject *parent=0);

	private:
		QList<ReporterInterface*> activeReporterPlugins() const;

		Settings *m_settings;

	public slots:
		void reportUsingPlugins(Result *result);

	signals:
		void done();

};

