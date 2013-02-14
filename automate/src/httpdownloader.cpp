#include "httpdownloader.h"

#include <QFileInfo>

#include <jb/std/log.h>
#include <jb/std/memory.h>

HTTPDownloader::HTTPDownloader(QObject *parent):
	Downloader(parent),
	http(NULL) {

}

void HTTPDownloader::downloadFile(std::string source, std::string destination) {
	// If an http object exists, let it finish and have the qt event loop delete it. important because http is async.
	DELETELATER(http)

	Downloader::downloadFile(source, destination);
	http = new QHttp(this);
	connect(http, SIGNAL(requestFinished(int, bool)),this, SLOT(requestFinished(int, bool)));
	connect(http, SIGNAL(dataReadProgress(int, int)),this, SIGNAL(dataReadProgress(int, int)));
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	connect(http, SIGNAL(readyRead(const QHttpResponseHeader&)), this, SLOT(writeToLocalFile(const QHttpResponseHeader&)));
// 	connect(http, SIGNAL(done(bool)), this, SIGNAL(downloadFinished(bool)));

	QHttp::ConnectionMode mode = url->scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;

	// Set hostname and port
	http->setHost(url->host(), mode, url->port() == -1 ? 0 : url->port());

	// Set authentication info
	if (!url->userName().isEmpty())
		http->setUser(url->userName(), url->password());

	// Prepare url path section (escape)
	QByteArray download_url = QUrl::toPercentEncoding(url->path(), "!$&'()*+,;=:@/");
	if (download_url.isEmpty())
		download_url = "/";

// 	LOG("State before starting download " << http->state()) // for debugging only
	getId = http->get(download_url);
}

void HTTPDownloader::requestFinished(int requestId, bool error) {

	std::string error_str  = error ? QString("Error %1").arg(error).toStdString() : "No error";
	LOG("Request finished " << requestId << " " << error_str);

	if (error) LOG("HTTP error " << http->errorString().toStdString());

	// Not the get request. ignore.
	if (requestId != getId) {
		LOG("Waiting for request " << getId << " to finish");
		return;
	}

	// Error on file download request. remove the file.
	if (error) {
		removeFile();
		emit downloadFinished(true);
		return;
	}

	// No error. finish writing and close local file.
	if (file) {
		file->write(http->readAll());
		file->close();
		DELETELATER(file);
		emit downloadFinished(false);
	}
}

void HTTPDownloader::readResponseHeader(const QHttpResponseHeader &responseHeader) {

	QString header,	/// Header
				 filename; /// Filename for downloaded file given by site.

	LOG("HTTP response: " << responseHeader.statusCode());

	switch (responseHeader.statusCode()) {
		case 301:                   // Moved Permanently
		case 302:                   // Found
		case 303:                   // See Other
		case 307:                   // Temporary Redirect
			header = responseHeader.value("Location");
			if (header.size() > 0) {
				// Issue request with new url.
				downloadFile(header.toStdString(), destination);
			}
			break;
		case 200:    // Ok
			// Check for attachment filename:
			header = responseHeader.value("content-disposition");
			if (header.size() > 0) {
				foreach (QString parameter, header.split(";", QString::SkipEmptyParts)) {
					parameter = parameter.trimmed();
					if (parameter.startsWith("filename=", Qt::CaseInsensitive)) {
						filename = parameter.section("=",1).trimmed();
						LOG("Got filename " << filename.toStdString());
					}
				}
			}
			// Download file
			if (!openDownloadFile(filename)) {
				LOG("Failed opening file for write")
				cancelDownload();
			}
			break;
		default:
			LOG("Unknown http response. Aborting")
			cancelDownload();
	}
}

void HTTPDownloader::writeToLocalFile(const QHttpResponseHeader& /* resp */) {

	// FIXME is buffer EVER used ?

	// No open file yet. write to buffer.
	if (!file) {
// 		qDebug() << "Writing to buffer";
		buffer.append(http->readAll());
		return;
	}

	// Empty buffer into file.
	if (buffer.length() != 0) {
// 		qDebug() << "Clearing buffer";
		file->write(buffer);
		buffer.clear();
	}

	file->write(http->readAll());
}

void HTTPDownloader::cancelDownload() {
	// NOTE must set this to true befure issuing abort.
	http->abort();
	Downloader::cancelDownload();
}
