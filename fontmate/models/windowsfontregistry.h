#pragma once

/** QT headers **/
#include <QMap>
#include <QString>
#include <QStringList>

namespace FontMate {

	class WindowsFontRegistry {
		public:
			WindowsFontRegistry();

			static QMap<QString, QString> registry_map;

			static QMap<QString, QString> registry_full_path;

			static QMap<QString, QString> registry_full_path_by_filename;

			static void load();

			static void clear();

			static QString registeredFontTitle(const QString &installed_font_full_path);

			/**
				@return List of full paths to registered fonts
			*/
			static QStringList registeredFontRealPaths();

			/**
				@return List of registered fonts as they appear in windows registry (These can be full paths, or relative to known system font paths).
			*/
			static QStringList registeredFontFiles();

			static QStringList registeredFontFileNames();

			/**
				Notify windows of changes in installed system fonts.

				@internal Should run in the background since it takes a while and otherwise blocks the UI.
			*/
			static void notifyChanges();

			static bool fontRegistered(const QString &non_installed_font_path);
	};

}
