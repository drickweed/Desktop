#pragma once

/** QT headers **/
#include <QString>

class DownloaderInterface {
	 public:
		 virtual ~DownloaderInterface() {}

		 virtual void downloadFile(QString url, QString destination);
		 virtual void cancelDownload();

	 protected:
		 virtual void updateDataTransferProgress(qint64 read, qint64 total);
		 virtual void commandFinished(int id, bool error);
		 virtual void commandStarted(int id);

};
