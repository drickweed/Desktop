/** QT headers **/
#include <QtTest/QtTest>
#include <QFile>

/** Application headers **/
#include "filechecksumrecord.h"

using namespace MiniSFV;

/**
	Test FileChecksumRecord.
*/
class Test_FileChecksumRecord: public QObject {
	Q_OBJECT

	private slots:
		void test_create_checksum_record();
		void test_checksum_record_tests();
		void test_empty_valid_record();
		void test_empty_invalid_record();
};

QTEST_MAIN(Test_FileChecksumRecord);

void Test_FileChecksumRecord::test_create_checksum_record() {

	FileChecksumRecord record("backup.log");
	QCOMPARE(record.path(), QString("backup.log"));
}

void Test_FileChecksumRecord::test_checksum_record_tests() {
	QFile test_file("test_file.txt");
	test_file.open(QIODevice::WriteOnly);
	test_file.close();

	unsigned long checksum = 12;

	FileChecksumRecord record("test_file.txt");
	QVERIFY(record.fileExists());
	QVERIFY(not record.hasStoredChecksum());
	QVERIFY(not record.hasCalculatedChecksum());
	QVERIFY(not record.allChecksumsKnown());
	QVERIFY(not record.isValid());
	QVERIFY(not record.isInvalid());
	QVERIFY(record.isUnknown());

	record.setCalculatedChecksum(12);
	QCOMPARE(record.calculatedChecksum(), checksum);
	QVERIFY(record.hasCalculatedChecksum());
	QVERIFY(not record.allChecksumsKnown());
	QVERIFY(not record.isValid());
	QVERIFY(not record.isInvalid());
	QVERIFY(record.isUnknown());

	record.setStoredChecksum(12);
	QCOMPARE(record.storedChecksum(), checksum);
	QVERIFY(record.hasStoredChecksum());
	QVERIFY(record.isValid());
	QVERIFY(not record.isInvalid());
	QVERIFY(not record.isUnknown());

	record.setStoredChecksum(13);
	QVERIFY(not record.isValid());
	QVERIFY(record.isInvalid());
	QVERIFY(not record.isUnknown());

	QVERIFY(record.allChecksumsKnown());
}

void Test_FileChecksumRecord::test_empty_valid_record(){
	FileChecksumRecord record("some/path");

	QVERIFY(record.valid());
	QVERIFY(not record.hasCalculatedChecksum());
	QVERIFY(not record.hasStoredChecksum());
}

void Test_FileChecksumRecord::test_empty_invalid_record() {
	FileChecksumRecord record;

	QVERIFY(not record.valid());
	QVERIFY(not record.hasCalculatedChecksum());
	QVERIFY(not record.hasStoredChecksum());
}

#include "test.moc"
