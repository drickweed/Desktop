#pragma once

/** QT headers **/
#include <QFtp>

#include "downloader.h"
#include <interface/download.h>

class FTPDownloader : public Downloader {
	Q_OBJECT
	Q_INTERFACES(DownloaderInterface)

	public:
		FTPDownloader(QObject* parent = 0);
		void downloadFile(std::string url, std::string destination);

	private slots:
		void updateDataTransferProgress(qint64 read, qint64 total);
		void commandFinished(int id, bool error);
		void commandStarted(int id);

	public slots:
		void cancelDownload();

	private:
		QFtp *ftp;
};
