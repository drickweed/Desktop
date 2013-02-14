#pragma once

#include <QtTest/QtTest>

class TestLibImp: public QObject {
	Q_OBJECT

	private slots:

		/** Person tests **/
		void newPersonWithSeparateNamesGiven();

		void newPersonWithCommaSeparator();
		void newPersonWithSpaceSeparator();
		void newPersonWithSingleName();
		void newPersonFromIndex();

		void displayNameStyles();

		/** Ordered person tests **/
		void newOrderedPerson();
		void orderedPersonTimeOrder();
		void selectOrderedPerson();

		/** Group tests **/
		void newGroupWithName();
		void groupWithChildren();
};
