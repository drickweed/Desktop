#include "removabledrive.h"

RemovableDrive::RemovableDrive(QObject *parent):
	QObject(parent) {
	m_process = new QProcess(this);
}

void RemovableDrive::findSerial(QString letter) {

}

void RemovableDrive::findLetter(QString uuid) {

}

