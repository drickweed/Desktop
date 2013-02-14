#pragma once

/** QT headers **/
#include <QObject>
#include <QSettings>
#include <QString>
#include <QStringList>

/** JB headers **/
#include <jb/models/options.h>

namespace FontMate {

	/**
		@author Shaar Gabriel <gabi@jbox-comp.com>
	*/
	class Settings : public QObject {
		Q_OBJECT
		public:
			Settings(QObject *parent=0);

			~Settings();

			QString defaultText();
			int defaultSize();
			QStringList fontPaths();
			int listFontSize();
			bool donated();
			bool saveUninstalledFonts();
			bool overwriteInstalledFonts();
			QString fontBackupFolder();
			bool logging();
			bool previewHighlighted();
			void restoreDefaultText();
			QString language();
			JB::OptionsModel *languages;
			QString translationFile();

			QString configFile() const;

			QSettings *settings;

			void sync();

		public slots:
			void setDefaultText(QString text);
			void setFontPaths(QStringList paths);
			void setDefaultSize(int size);
			void setListFontSize(int size);
			void setDonated(bool donated);
			void setSaveUninstalledFonts(bool save);
			void setOverwriteInstalledFonts(bool overwrite);
			void setFontBackupFolder(QString folder);
			void setLogging(bool enabled);
			void setPreviewHighlighted(bool enabled);
			void setLanguage(QString language);

		signals:
			void donated(bool donated);
			void languageChanged(QString language_id);

	};

}
