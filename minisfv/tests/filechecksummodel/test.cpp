/** QT headers **/
#include <QtTest/QtTest>

/** Application headers **/
#include "filechecksummodel.h"

using namespace MiniSFV;

class Test_FileChecksumModel: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void cleanupTestCase();
		void init();
		void cleanup();

		void test_create_model();
		void test_add_model_items();
		void test_add_model_items_with_checksum();
		void test_load_urls();
		void test_ignore_non_existing_files_with_load_urls();

	private:
		FileChecksumModel *model;
};

void Test_FileChecksumModel::initTestCase() {
}

void Test_FileChecksumModel::cleanupTestCase() {
}

void Test_FileChecksumModel::init() {
	model = new FileChecksumModel();
}

void Test_FileChecksumModel::cleanup() {
	delete model;
	model = NULL;
}

QTEST_MAIN(Test_FileChecksumModel);

void Test_FileChecksumModel::test_create_model() {
	QCOMPARE(model->rowCount(), 0);
}

void Test_FileChecksumModel::test_add_model_items() {
	model->add("/home/tlailax/backup.log");

	QCOMPARE(model->rowCount(), 1);
	QCOMPARE(model->getRecord(0).path(), QString("/home/tlailax/backup.log"));
}

void Test_FileChecksumModel::test_add_model_items_with_checksum() {
	int index = model->add("/home/tlailax/backup.txt");
	model->setChecksum(index, 123);

	QVERIFY(model->getRecord(index).hasCalculatedChecksum());
	QCOMPARE(model->getRecord(index).calculatedChecksum(), (unsigned long)123);

}

void Test_FileChecksumModel::test_load_urls() {
	QStringList urls = QStringList() << "/home/tlailax/pixmaps";
	model->loadUrls(urls);

	QCOMPARE(model->basePath(), QString("/home/tlailax/pixmaps/"));
}

void Test_FileChecksumModel::test_ignore_non_existing_files_with_load_urls() {
	QStringList urls = QStringList() << "/home/imaginary_file.txt" << "/home/another_imaginary_file.txt";
	model->loadUrls(urls);

	// Model will ignore non existing files
	QCOMPARE(model->rowCount(), 0);
}


#include "test.moc"
