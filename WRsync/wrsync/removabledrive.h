#pragma once

/** QT headers **/
#include <QObject>
#include <QProcess>

class RemovableDrive : public QObject {
	Q_OBJECT
	public:
		RemovableDrive(QObject *parent = 0);

		void setSerial(QString serial) { m_serial = serial; }
		QString Serial() const { return m_serial; }

		void findSerial(QString letter);
		void findLetter(QString uuid);

	private:
		QString m_serial;
		QString m_letter;
		QProcess *m_process;

	signals:

	public slots:

};
