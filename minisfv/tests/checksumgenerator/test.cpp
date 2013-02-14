/** QT headers **/
#include <QtTest/QtTest>

/** Application headers **/
#include "crc32checksum.h"

using namespace MiniSFV;

class Test_ChecksumGenerator: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void cleanupTestCase();
		void init();
		void cleanup();

		void test_checksum_generate();

};

void Test_ChecksumGenerator::initTestCase() {
}

void Test_ChecksumGenerator::cleanupTestCase() {
}

void Test_ChecksumGenerator::init() {
}

void Test_ChecksumGenerator::cleanup() {
}

QTEST_MAIN(Test_ChecksumGenerator);

void Test_ChecksumGenerator::test_checksum_generate() {
	CRC32Checksum checksum_generator("sample.xcf");
	unsigned long bytes_read;
	unsigned long result = checksum_generator.generate(bytes_read);

}

#include "test.moc"
