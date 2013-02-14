#include "test_libimp.h"

#include <QDebug>

#include "person.h"
#include "orderedperson.h"
#include "group.h"


using namespace ImageImp;

/** Person tests **/

void TestLibImp::newPersonWithSeparateNamesGiven() {
	Person *person = new Person("first", "last", "01232");
	QCOMPARE(person->firstName(), QString("first"));
	QCOMPARE(person->lastName(), QString("last"));
	QCOMPARE(person->id(), QString("01232"));

	QCOMPARE(person->fullName(), QString("first last"));
	Person::setFirstNameFirst(false);
	QCOMPARE(person->fullName(), QString("last first"));
	delete person;
}

void TestLibImp::displayNameStyles() {
	Person *person = new Person("first", "last", "01232");

	Person::setFirstNameFirst(true);
	QCOMPARE(person->fullName(), QString("first last"));
	Person::setFirstNameFirst(false);
	QCOMPARE(person->fullName(), QString("last first"));
	delete person;
}

void TestLibImp::newPersonWithCommaSeparator() {
	Person *person = new Person("last, first", "01232");
	QCOMPARE(person->firstName(), QString("first"));
	QCOMPARE(person->lastName(), QString("last"));
	QCOMPARE(person->id(), QString("01232"));
	delete person;
}

void TestLibImp::newPersonWithSpaceSeparator() {
	Person *person = new Person("last first", "01232");
	QCOMPARE(person->firstName(), QString("first"));
	QCOMPARE(person->lastName(), QString("last"));
	QCOMPARE(person->id(), QString("01232"));
	delete person;
}

void TestLibImp::newPersonWithSingleName() {
	Person *person = new Person("single", "01232");
	QCOMPARE(person->firstName(), QString("single"));
	QCOMPARE(person->lastName(), QString(""));
	QCOMPARE(person->id(), QString("01232"));
	delete person;
}

void TestLibImp::newPersonFromIndex() {
	Person *person = Person::new_from_index(45);
	QCOMPARE(person->id(), QString("000000045"));
	QCOMPARE(person->firstName(), QString("First"));
	QCOMPARE(person->lastName(), QString("Last"));
	delete person;
}

/** Ordere person tests **/

void TestLibImp::newOrderedPerson() {
	Person person("single", "01232");
	OrderedPerson *ordered_person = new OrderedPerson(person);
	QCOMPARE(ordered_person->firstName(), QString("single"));
	QCOMPARE(ordered_person->lastName(), QString(""));
	QCOMPARE(ordered_person->id(), QString("01232"));
	delete ordered_person;
}

void TestLibImp::orderedPersonTimeOrder() {
	Person person("single", "01232");
	OrderedPerson *ordered_person_older = new OrderedPerson(person, "20080101T200100");
	OrderedPerson *ordered_person_newer = new OrderedPerson(person, "20080101T200530");
	QVERIFY(*ordered_person_older < *ordered_person_newer);
	delete ordered_person_older;
	delete ordered_person_newer;
}

void TestLibImp::selectOrderedPerson() {
	Person person("single", "01232");
	OrderedPerson *ordered_person = new OrderedPerson(person);
	// Selected by default
	QCOMPARE(ordered_person->selected(), true);
	// Unselect
	ordered_person->setSelected(false);
	QCOMPARE(ordered_person->selected(), false);
	delete ordered_person;
}


/** Group tests **/

void TestLibImp::newGroupWithName() {
	Group *group = new Group("first group");
	QCOMPARE(group->name(), QString("first group"));
	delete group;
}

void TestLibImp::groupWithChildren() {
	Group *group = new Group("first group");
	Person *person = new Person("last first", "123");
	person->setParent(group);
	delete group;
	delete person;
}

QTEST_MAIN(TestLibImp)
#include "test_libimp.moc"
