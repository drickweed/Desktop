#include "person.h"

namespace ImageImp {

	Person::Person(QString name):
		RelationalObject(){
		setName(name);
	}

	void Person::setDefaults() {
		RelationalObject::setDefaults();
		setName("New person");
		setSid("000000000");
	}

	void Person::removeDependencies() {
		deleteRelated("photodates");
		deleteRelated("pictures");
		deleteRelated("ordered_people", "item_id");
		deleteRelated("excluded_people", "item_id");
	}

}
