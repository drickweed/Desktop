#pragma once

#include <QHttpResponseHeader>
#include <QByteArray>

#include "downloader.h"

class HTTPDownloader : public Downloader {
	Q_OBJECT

	public:
		HTTPDownloader(QObject* parent = 0);
		void downloadFile(std::string url, std::string destination);
	private slots:

		void requestFinished(int requestId, bool error);
		void readResponseHeader(const QHttpResponseHeader &header);
		void writeToLocalFile(const QHttpResponseHeader &header);
	public slots:
		void cancelDownload();

	private:
		QHttp *http;
		int getId;
		QByteArray buffer;
};
