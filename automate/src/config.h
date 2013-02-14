#pragma once

#include <string>
#include <QSettings>
#include <map>

#include "const.h"

/**
	Global configuration options.

	Set/Get config options and save to config file in application folder.

	NOTE Used as a singleton.
*/
class Config : public QObject {
	Q_OBJECT

	public:
		/**
			Configuration options.
		*/
		enum Fields {Database, Overwrite, ShowHelp, StopProcessingOn, SilentOrder};

		/**
			Init config object.
		*/
		static void init(QString path, QObject *parent=NULL);

		/**
			Single config instance.
		*/
		static Config& Instance() {
			static Config conf;
			return conf;
		}

		/**
			Database path.

			@return database path. Default to "<application folder>/database"
		*/
		std::string database() const;

		/**
			Set database path.

			@param path path to application and tweak database.
		*/
		void setDatabase(std::string path);

		/**
			Overwrite files when downloading.

			@return True if overwrite. Default to false.
		*/
		bool overwrite() const;

		/**
			Set overwrite files when downloading.

			@param value True if overwrite.
		*/
		void setOverwrite(bool value);

		/**
			Show quick help on startup.

			@return True if shown. Default to true.
		*/
		bool showHelp() const;

		/**
			Set show quick help on startup.

			@param value True if shown.
		*/
		void setShowHelp(bool value);

		/**
			Get what error to stop on.

			@return What error to stop processing installation batch on. See ProcessingError::ProcessingErrorType enum for details. Default to not stop on any error.
		*/
		ProcessingError::ProcessingErrorType stopProcessingOn() const;

		/**
			Set when to stop processing installation batch.

			@param value When to stop processing installations batch. See ProcessingError::ProcessingErrorType enum for details).
		*/
		void setStopProcessingOn(ProcessingError::ProcessingErrorType value);

		/**
			Halt on download error.

			@return True if halt on download error.
		*/
		bool haltOnDownloadError() const {
			return (stopProcessingOn() == ProcessingError::Download or stopProcessingOn() == ProcessingError::Any);
		}

		/**
			Halt batch installation process on error.

			@return True if halt on error in installation.
		*/
		bool haltOnInstallError() const {
			return (stopProcessingOn() == ProcessingError::Install or stopProcessingOn() == ProcessingError::Any);
		}

		/**
			Check if silent installs should apply first.

			@return True if silent installers run first.
		*/
		bool silentFirst() const {
			return silent() == SilentOrder::First;
		}

		/**
			Get silet install order.

			@return What type of installers to run first. Default to run silent installers first.
		*/
		SilentOrder::SilentOrderTypes silent() const {
			return static_cast<SilentOrder::SilentOrderTypes>(settings->value(lookup[Config::SilentOrder], SilentOrder::First).toInt());
		}

		/**
			Set installation order of silent installers.

			@param order order of silent installations. See SilentOrder::SilentOrderTypes enum for details.
		*/
		void setSilent(SilentOrder::SilentOrderTypes order);

		void restoreDefaults() { settings->clear(); }

	public slots:
		/**
			Sync settings to file.
		*/
		void sync();

	private:

		// These methods are private to uphold the singleton pattern.
		Config() {}
		Config(Config const&);
		Config& operator= (Config const&);
		~Config() {}

		static QSettings *settings; /// Qt settings manager
		static std::map<int, QString> lookup; /// Lookup table

	// FIXME find why signal from singleton doesn't work
// 	signals:
// 		void databaseLocationChanged();
};

// Shortcut
#define CONFIG Config::Instance()

