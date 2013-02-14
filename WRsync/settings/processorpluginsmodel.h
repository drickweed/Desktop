#pragma once

/** QT headers **/
#include <QAbstractListModel>

/** Application headers **/
#include <plugins/interface/processor.h>

class ProcessorPluginsModel : public QAbstractListModel {
	Q_OBJECT
	public:
		explicit ProcessorPluginsModel(QObject *parent = 0);

		/** QT api **/
//		Qt::ItemFlags flags ( const QModelIndex & index ) const;

		/** QT api **/
		QVariant data ( const QModelIndex & index, int role=Qt::DisplayRole ) const;

		/** QT api **/
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

		QWidget* configUi(int index) {
			return processor(index)->configUi();
		}

		ProcessorInterface *processor(int index) const;

		int indexOf(QString plugin_id) const;

		void setPlugins(QList<ProcessorInterface*> plugins);

		void savePluginSettings();

	private:
		QMap<QString, int> m_plugin_ids;
		QList<ProcessorInterface*> m_plugins;

};
