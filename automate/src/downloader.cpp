// #include <QDebug>

#include <boost/filesystem.hpp>

#include <iostream>

#include <QFileInfo>

#include <jb/std/memory.h>
#include <jb/std/log.h>

#include "downloader.h"
#include "config.h"

Downloader::Downloader(QObject *parent):
	QObject(parent),
	file(NULL),
	url(NULL)	 {
}

bool Downloader::openDownloadFile(QString filename) {

	// Check destination path exists
	if (!validatePath())
		return false;

	// Get filename from url if not given as parameter
	QFileInfo fileInfo(url->path());
	if (filename.isEmpty())
		filename = fileInfo.fileName();

	// Create full filename
	QString full_filename = (boost::filesystem::path(destination) /  filename.toStdString()).string().c_str();

	LOG("Downloading to " << full_filename.toStdString());
	// Remove existing file
	if (QFile::exists(full_filename)) {
		if (!CONFIG.overwrite()) {//overwrite check
			LOG("File exists. not overwriting");
			return false;
		}
		QFile::remove(full_filename);
	}

	file = new QFile(full_filename, this);

	// Open local file for writing.
	if (!file->open(QIODevice::Truncate|QIODevice::WriteOnly)) {
		DELETELATER(file);
		return false;
	}

	emit currentDownloadFilename(filename);
	return true;
}

bool Downloader::validatePath() {
	if (!boost::filesystem::exists(boost::filesystem::path(destination)))
		return boost::filesystem::create_directory(boost::filesystem::path(destination));
	return true;
}

void Downloader::removeFile() {
	if (file and file->exists()) {
		LOG("Removing file");
		file->remove();
	}
}

void Downloader::cancelDownload() {
	LOG("Canceling download");
}

void Downloader::downloadFile(std::string source, std::string destination) {
	this->destination = destination;
	url = new QUrl(source.c_str());
	LOG("Downloading " << url->toString().toStdString());
}
