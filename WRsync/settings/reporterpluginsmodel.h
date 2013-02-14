#pragma once

/** QT headers **/
#include <QAbstractListModel>
#include <QStringList>
#include <QMap>
#include <QSettings>

/** Application headers **/
#include <interface/reporter.h>
#include <wrsync/settings.h>

using namespace WRsync;

class ReporterPluginsModel : public QAbstractListModel {
	Q_OBJECT
	public:
		ReporterPluginsModel(QObject *parent=0);

		/** QT api **/
		Qt::ItemFlags flags ( const QModelIndex & index ) const;

		/** QT api **/
		QVariant data ( const QModelIndex & index, int role=Qt::DisplayRole ) const;

		/** QT api **/
		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

		/** QT api **/
		bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

		void savePluginSettings();
		void saveSettings(Settings *settings);
		void loadSettings(Settings *settings);

		void setPlugins(QList<ReporterInterface*> plugins);

		QWidget* configUi(int index) {
			return reporter(index)->configUi();
		}

		QString name(int index) {
			return reporter(index)->name();
		}

		void setEnabled(int index, bool enabled) {
			setEnabled(reporter(index)->id(), enabled);
		}

		void setEnabled(QString id, bool enabled) {
			m_enabled_plugins[id] = enabled;
		}

		bool enabled(int index) const {
			return enabled(reporter(index)->id());
		}

		bool enabled(QString id) const {
			return m_enabled_plugins.value(id, false);
		}

		QStringList enabledPlugins() const {
			return m_enabled_plugins.keys(true);
		}

		void setEnabledPlugins(QStringList enabled_plugins) {
			foreach(QString plugin, enabled_plugins) {
				m_enabled_plugins[plugin] = true;
			}
		}

	private:
		QMap<QString, bool>	m_enabled_plugins;
		QMap<QString, ReporterInterface*> m_plugins;

		ReporterInterface *reporter(int index) const;

};

