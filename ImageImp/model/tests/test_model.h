#pragma once

#include <QtTest/QtTest>

#include "imp/group.h"
#include "model/groups.h"

using namespace ImageImp;

class TestModel: public QObject {
	Q_OBJECT

	private:

		Group *group;
		Groups *groups;

	private slots:

		void initTestCase();
		void cleanupTestCase();
		void init();
		void cleanup();

		/** Group model **/
		void test_new_group_model();
		void test_new_group_in_model();
		void test_add_a_lot_of_groups_to_model();
};
