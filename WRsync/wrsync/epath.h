#pragma once

/** QT headers **/
#include <QString>
#include <QChar>
#include <QList>

/** Application headers **/
#include "global.h"

const QString REMOVABLE_URI = "removable://";
const QString SSH_URI = "ssh:";
const QString MSSQL_URI = "mssql://";

/**
	Extended path class.
*/
class WRSYNC_API EPath {
	public:

		typedef enum {Path, Include, Exclude, Comment} SourceType;

		EPath();
		EPath(QString path);

		QString path() const;

		QString unixPath() const;

		QString cygwinPath(QChar drive_letter) const;

		QString realPath() const;

		void setPath(QString path) {
			m_path = path;

			// Attach to removable drive
			connectRemovableDevice();
		}

		bool connectRemovableDevice() {
			if (isRemovable()) {
				setDriveByID(removableId());
			}
			return not m_usable_drive.isNull();
		}

		SourceType type() const {
			if (path().startsWith('-')) return Exclude;
			if (path().startsWith('+')) return Include;
			if (path().startsWith('#')) return Comment;
			return Path;
		}

		bool isEmpty() const { return m_path.isEmpty(); }

		bool isValid() const { return isValid(m_path); }

		static bool isValid(QString path);

		void setShadowVolume(QString shadow_volume) { m_shadow_volume = shadow_volume; }
		QString shadowVolume() const { return m_shadow_volume; }

		QString shadowPath() const;

		void mountShadow();
		void umountShadow();

		QString driveID() const;

		/**
			Get partition ID by drive letter

			@param drive_letter Drive letter of volume to identify
			@return Partition ID or empty string if not a usb device
		*/
		static QString driveID(QChar drive_letter);

		void setDriveByID(QString id);

		bool isNetworkPath() const { return m_path.startsWith("\\\\"); }
		bool isLocalPath() const { return m_path[1] == ':' and m_path[0].isLetter(); }
		bool isRSyncPath() const { return m_path.toLower().startsWith(SSH_URI); }
		bool isRemovable() const { return m_path.toLower().startsWith(REMOVABLE_URI); }
		bool isMsSql() const { return m_path.toLower().startsWith(MSSQL_URI); }

		QString removableRelativePath() const;
		QString removableId() const;

		bool attachToDevice(QStringList device_ids);

		bool matchesDiskID(QString id) const;
		bool matchesDiskID(QStringList ids) const;

		QChar realDriveLetter() const { return m_path[0]; }
		QChar driveLetter() const;
		void setDriveLetter(QChar drive) { m_usable_drive = drive; }

		static QChar driveLetterById(QString id);

		/**
			Gets list of all drive letters.

			This includes:
			- Network mapped drives
			- Drives mapped using 'subst' command
			- Removable devices
			- Static devices

			@return List of drive active drive letters
		*/
		static QList<QChar> existingDriveLetters();

		static QList<QChar> availableDriveLetters();

		static QChar nextAvailableDrive() { return availableDriveLetters()[0]; }

		/**
			Mount network path and modify the path to return the local version
		*/
		bool mountNetwork() { return mountNetwork(EPath::nextAvailableDrive()); }

		/**
			Mount network path and modify the path to return the local version

			@param drive local drive to mount network path to.
		*/
		bool mountNetwork(QChar drive);

		/**
			Umount network drive.
		*/
		bool unmountNetwork();

		void setLastDeviceId(QString id) { m_last_known_device_id = id;}
		QString lastDeviceId() const { return m_last_known_device_id; }

		bool operator== (EPath &other) const;

		bool prepareDatabaseFiles();

	private:
		// Native operating system formatted absolute local filesystem path, or a filter line (starts with - or +)
		QString m_path,
				m_shadow_volume;
		bool m_remote_path_mounted;
		QChar m_shadow_drive, m_usable_drive;
		QString m_last_known_device_id;

};

