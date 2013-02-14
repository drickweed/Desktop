/** QT headers **/
#include <QObject>
#include <QTimer>
#include <vector>

/** JB headers **/
#include <jb/std/hdatetime.h>

/** Application headers **/
#include "version.h"
#include "tweak.h"
#include "tweakrunner.h"
#include "ftpdownloader.h"
#include "httpdownloader.h"
#include "installer.h"

const int WAIT_FOR_DOWNLOAD_DATA = 2000; /// 2 seconds


class Controller : public QObject {
	Q_OBJECT

	private:

		Downloader::TransferType download_type;

		FTPDownloader ftp_downloader; /// FTP file downloader
		HTTPDownloader http_downloader; /// HTTP file downloader

		Installer installer; /// Application installer
		TweakRunner tweakRunner; /// Tweak running utility

		std::vector<Version> m_versions; /// Versions
		std::vector<TweakAction> m_tweaks; /// Tweaks

		bool m_abort; /// Stop working when done with current job
		bool m_download_only; /// Only download applications

		bool m_busy; /// Is the controller busy doing something ?

		Version currentVersion; /// Version currently being applied

		int lastDownloadSize, currentDownloadSize; /// Last download size.

		QTime last_speed_update;

		QTimer *speedUpdateTimer; /// Time indicating the ui should be updated with zero speed since nothing has beedn downloaded recently. recently is defined in the constant WAIT_FOR_DOWNLOAD_DATA as a number of seconds.

	public:
		Controller(QObject *parent=NULL);
		~Controller();

		/**
			Check if the controller is busy

			@return True if busy
		*/
		bool busy() { return m_busy; }

		/**
			Download single version.

			@param version version to download
		*/
		void download(Version &version);

		/**
			Internal controller connections.
		*/
		void connectController();

		/**
			Load tweaks.
		*/
		static std::vector<Tweak> loadTweaks();

	public slots:
		/**
			Start processing versions.

			@param version version to process
			@param download_only only download files
		*/
		void processApplications(std::vector<Version> versions, bool download_only);

		/**
			Apply tweaks.

			@param tweaks tweak actions to apply.
		*/
		void processTweaks(std::vector<TweakAction> tweaks);

		/**
			Prevent the next download from starting.

			Allows current download to finish.
		*/
		void abort();

	private slots:
		/**
			Install next requested application.
		*/
		void installNext();

		/**
			Cleanup after current installer.
		*/
		void cleanupAfterInstall();

		/**
			Apply next tweak.
		*/
		void applyNext();

		/**
			Apply current tweak.
		*/
		void apply();

		/**
			Install current application version.

			@param error was there an error downloading the application.
		*/
		void install(bool error=false);

		/**
			Cleanup operations when controller is done processing queue
		*/
		void done();

		void prepare();

		void signalSpeed();
		void updateSpeed(int done, int total);

	signals:
		/**
			Downloads finished properly.

			@param error there was an error
		*/
		void finished(bool error);

		/**
			Downloads aborted by user.
		*/
		void aborted();

		void dataReadProgress(int done, int total);

		void downloadSpeed(int kbps);

		void startedNewDownload();

		void installDone();

		/**
			Reports current status

			@param status status message
		*/
		void status(QString message);

		void started();

		void currentProcess(QString name);
};
