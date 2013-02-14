#include "jobstatemodel.h"

/** Application headers **/
#include <wrsync/pluginmanager.h>
#include <wrsync/logfile.h>
#include "messages.h"

JobStateModel::JobStateModel(QObject *parent) :
	QAbstractTableModel(parent) {
}

QVariant JobStateModel::data(const QModelIndex &index, int role) const {

	if (not index.isValid())
		return QVariant();

	int row = index.row();
	int column = index.column();
	Result result = m_results[row];

	switch (role) {
		case Qt::DisplayRole:
			switch (column) {
				case 0: return QVariant(result.sourcePath());
				case 1:
					switch (result.state()) {
						case Result::Error :
						return QVariant(QString("rsync: %1").arg(Messages::rsync(result.exitCode())));
						default:
							return QVariant(Messages::state(result.state()));
					}


			}
			break;
		default:
			break;
	}

	return QVariant();
}

QVariant JobStateModel::headerData ( int section, Qt::Orientation orientation, int role) const {

	switch (role) {
		case Qt::DisplayRole:
			if (orientation == Qt::Horizontal)
				switch (section) {
					case 0: return QVariant(tr("Source"));
					case 1: return QVariant(tr("Result"));
				}
			break;
		default:
			break;
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

int JobStateModel::rowCount ( const QModelIndex & parent) const {
	return m_results.count();
}

void JobStateModel::loadDayResults(QDate day) {
	m_results.clear();

	foreach(ReporterInterface* plugin, PluginManager::reporterPlugins()) {
		if (plugin) {
			m_results.append(plugin->resultsForDay(day));
		}
	}

	reset();
}
