#pragma once

#include <QtTest/QtTest>

#include "imp/group.h"

using namespace ImageImp;

class TestImp: public QObject {
	Q_OBJECT

	private:

		Group *group;

	private slots:

		void init();
		void cleanup();

		/** Group **/

		void test_new_group();
		void test_group_defaults();
		void test_group_table();
		void test_group_with_init();


};
