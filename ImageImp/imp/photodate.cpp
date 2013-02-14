#include "photodate.h"

#include <QDebug>

namespace ImageImp {

	PhotoDate::PhotoDate(Person *person):
		RelationalObject(){
		setDefaults();
		if (person) {
			setPersonId(person->id());
		}
		setDateTime(QDateTime::currentDateTime());
	}

}
