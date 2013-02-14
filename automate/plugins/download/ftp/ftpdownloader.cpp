#include "ftpdownloader.h"

/** QT headers **/
#include <QFileInfo>

/** JB headers **/
#include <jb/std/log.h>
#include <jb/std/memory.h>

FTPDownloader::FTPDownloader(QObject *parent) :
	Downloader(parent) {
	ftp = new QFtp(this);
	connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(updateDataTransferProgress(qint64, qint64)));
	connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(commandFinished(int, bool)));
	connect(ftp, SIGNAL(commandStarted(int)), this, SLOT(commandStarted(int)));
	connect(ftp, SIGNAL(done(bool)), this, SIGNAL(downloadFinished(bool)));
}

void FTPDownloader::downloadFile(std::string source, std::string destination) {
	Downloader::downloadFile(source, destination);
	if (!openDownloadFile(QFileInfo(url->path()).fileName())) {
		emit downloadFinished(true);
		return;
	}
	if (ftp->state() != QFtp::Unconnected)
		ftp->close();
	ftp->connectToHost(url->host());
	ftp->login();
	ftp->get(url->path(), file);
	ftp->close();
}

void FTPDownloader::commandStarted(int id) {
	LOG("Started command " << id);
}

void FTPDownloader::commandFinished(int id, bool error) {

	LOG("Finished command " << id);
	if (error) {
		switch (ftp->currentCommand()) {
			case QFtp::Login:
				LOG("FTP login error");
				break;
			case QFtp::Get:
				LOG("FTP file get error");
				break;
			default:
				LOG("FTP error " << ftp->currentCommand() << " (lookup in qt reference QFtp)");
		}
		removeFile();
		return;
	}

	if (ftp->currentCommand() == QFtp::Login) {
		LOG("Logged into ftp");
		return;
	}

	if (ftp->currentCommand() == QFtp::Get) {
		return;
	}

	if (ftp->currentCommand() == QFtp::Close) {
		file->close();
		DELETELATER(file);
		LOG("File downloaded.");
		return;
	}
}

void FTPDownloader::cancelDownload() {
	ftp->abort();
	if (ftp->state() != QFtp::Unconnected) {
		LOG("Disconnecting from FTP. please wait.");
		ftp->close();
	}
	Downloader::cancelDownload();
}

void FTPDownloader::updateDataTransferProgress(qint64 read, qint64 total) {
	emit dataReadProgress((int) read, (int) total);
}
