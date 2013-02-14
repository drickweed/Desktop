#pragma once

/** QT headers **/
#include <QPushButton>
#include <QAction>
#include <QFtp>
#include <QList>
#include <QString>
#include <QFile>

/** Application headers **/
#include "target.h"

class TargetButton : public QPushButton, public Target {
	Q_OBJECT

	public:
		TargetButton(QString full_path, QWidget *parent=0);
		void setImageCount(int count) { m_image_count = count; updateButtonText(); }
		void incrementImageCount();
		void updateButtonText();
		int imageCount() const { return m_image_count; }
		void setFtpInfo(QString host, QString user, QString password, int port=21);
		QAction *uploadAction, *viewAction, *removeAction;

	private slots:
		void emitClickedWithPath();
		void targetMenu(QPoint position);
		void cleanupTransfer(bool error);
		void cleanupCommand(int id, bool error);
		void uploadNextFile();
		void upload();

	private:
		QFtp *ftp;
		QFile *upload_file;
		int upload_id;
		int done_id;
		QString m_host, m_user, m_password;
		int m_port;
		int m_image_count;
		QList<QString> files;

	signals:
		void clicked(QString path);
		void uploadDone(int error_code);
		void uploadMessage(QString message);
};
