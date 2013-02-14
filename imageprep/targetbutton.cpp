#include "targetbutton.h"

/** QT headers **/
#include <QMenu>
#include <QDir>
#include <QFile>

TargetButton::TargetButton(QString full_path, QWidget *parent):
	QPushButton(parent),
	Target(full_path),
	ftp(NULL),
	upload_file(NULL),
	upload_id(-1),
	done_id(-1),
	m_image_count(0) {

	setContextMenuPolicy(Qt::CustomContextMenu);
	updateButtonText();
	connect(this, SIGNAL(clicked()), SLOT(emitClickedWithPath()));
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(targetMenu(QPoint)));

//	uploadAction = new QAction(tr("Upload"), this);
//	connect(uploadAction, SIGNAL(triggered()), SLOT(upload()));
	viewAction = new QAction(tr("View"), this);
	removeAction = new QAction(tr("Remove"), this);

	ftp = new QFtp(this);
	connect(ftp, SIGNAL(commandFinished(int,bool)), SLOT(cleanupCommand(int,bool)));
}

void TargetButton::emitClickedWithPath() {
	emit clicked(path());
}

void TargetButton::incrementImageCount() { m_image_count++; updateButtonText(); }

void TargetButton::updateButtonText() {
	setText(QString("%1 (%2)").arg(name()).arg(imageCount()));
}

void TargetButton::targetMenu(QPoint /* position */) {
	QMenu menu(this);
//	menu.addAction(uploadAction);
	menu.addAction(viewAction);
	menu.addAction(removeAction);

	menu.exec(mapToGlobal(QPoint(0,0)));
}

void TargetButton::setFtpInfo(QString host, QString user, QString password, int port) {
	m_host = host;
	m_user = user;
	m_password = password;
	m_port = port;
}

void TargetButton::upload() {
	if (ftp != NULL)
		return;

	files = QDir(path()).entryList(QDir::Files | QDir::NoSymLinks);
	ftp->connectToHost(m_host, m_port);
	ftp->login(m_user, m_password);
	uploadNextFile();
}

void TargetButton::cleanupCommand(int id, bool error) {
	if (ftp == NULL)
		return;

	if (id == upload_id) {
		if (files.empty()) {
			done_id = ftp->close();
			return;
		}
		uploadNextFile();
	}

	if (id == done_id)
		cleanupTransfer(error);
}

void TargetButton::uploadNextFile() {
	if (ftp == NULL)
		return;

	if (upload_file != NULL) {
		upload_file->deleteLater();
		upload_file = NULL;
	}

	QString file_name = files.takeFirst();
	upload_file = new QFile(fullPath(file_name));
	upload_id = ftp->put(upload_file, file_name);
//	emit uploadingFile(file_name);
}

void TargetButton::cleanupTransfer(bool /* error */) {

	if (ftp == NULL)
		return;

	ftp->deleteLater();
	ftp = NULL;
}
