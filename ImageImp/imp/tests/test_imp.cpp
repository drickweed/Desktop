#include "test_imp.h"

#include <QDebug>
#include <exception>
#include <QFile>

using namespace ImageImp;
using namespace JB;

QTEST_MAIN(TestImp);


void TestImp::init() {
	group = new Group;
}

void TestImp::cleanup() {
	delete group;
}

void TestImp::test_new_group() {
	group->setName("Group A");
	QCOMPARE(group->name(), QString("Group A"));
}

void TestImp::test_group_defaults() {
	group->setDefaults();
	QCOMPARE(group->name(), QString("New group"));
}

void TestImp::test_group_table() {
	QCOMPARE(Group().table(), QString("groups"));
}

void TestImp::test_group_with_init() {
	delete group;
	group = new Group("Test group");
	QCOMPARE(group->name(), QString("Test group"));
}

