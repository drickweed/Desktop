#include "test_model.h"

#include <QDebug>
#include <exception>
#include <QFile>

#include "jb/qt4/database/database.h"

using namespace ImageImp;
using namespace JB;

QTEST_MAIN(TestModel);

void TestModel::initTestCase() {

}

void TestModel::cleanupTestCase() {
}

void TestModel::init() {
	QString db_path = "imptest.sqlite3";

	// Remove old database file. NOTE removed here and not at the end of the test to be able to debug failed tests.
	QFile::remove(db_path);

	try {
		Database::init(db_path, "/home/tlailax/Projects/ImageImp/resources/imageimp.sql");
	} catch (std::exception *e) {
		qDebug() << e->what();
	}

	group = new Group;
	groups = new Groups;
}

void TestModel::cleanup() {
	delete group;
	delete groups;
}

void TestModel::test_new_group_in_model() {
	groups->appendNew();
	QCOMPARE(groups->rowCount(QModelIndex()), 1);
	QCOMPARE(groups->record(0).value("name").toString(), QString("New group"));
	group->load(groups->record(0));
	QCOMPARE(group->name(), QString("New group"));
	QCOMPARE(groups->rowCount(), 1);
}

void TestModel::test_new_group_model() {
	QCOMPARE(groups->tableName(), QString("groups"));
}

void TestModel::test_add_a_lot_of_groups_to_model()
{
	for(int i=0;i<10;i++)
		groups->appendNew();
	QCOMPARE(groups->rowCount(), 10);
}
