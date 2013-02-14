#pragma once

#include <QUrl>
#include <QFile>
#include <QObject>
#include <string>

/**
	File downloader.

	HTTP and FTP file downloader.
*/
class Downloader : public QObject {
	Q_OBJECT

	public:

		enum TransferType {HTTP, FTP};

		Downloader(QObject *parent = 0);

		/**
			Start downloading file.
		*/
		virtual void downloadFile(std::string url, std::string destination);

	protected slots:

		/**
			Open destination file for writing.

			@param filename filename to write download to
			@return success
		*/
		bool openDownloadFile(QString filename = QString());

	protected:

		QFile *file; ///
		QUrl *url; ///
		std::string destination; ///

		/**
			Ensure existance of local destination folder.
		*/
		bool validatePath();

		void removeFile();

	public slots:
		/**
			Cancel download file.

			This is used for both HTTP and FTP.
		*/
		virtual void cancelDownload();

	signals:

		/**
		*/
		void dataReadProgress(int, int);

		/**
		*/
		void downloadFinished(bool);

		void currentDownloadFilename(QString filename);
};
