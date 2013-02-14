#include "about.h"
#include <QDesktopServices>

namespace FontMate {

	About::About(QWidget* parent, Qt::WFlags fl):
		QDialog(parent, fl),
		Ui::Dialog() {
		setupUi(this);

		connect(donateButton, SIGNAL(clicked()), SLOT(donate()));
		connect(donateLaterButton, SIGNAL(clicked()), SLOT(donateLater()));
		connect(alreadyDonatedButton, SIGNAL(clicked()), SLOT(alreadyDonated()));
		connect(siteButton, SIGNAL(clicked()), SLOT(openSite()));
	}

	About::~About() {
	}

}

void FontMate::About::openSite() {
	QDesktopServices::openUrl(QUrl("http://www.jbox-comp.com/fontmate/"));
	done(DonateLater);
}

void FontMate::About::donate() {
	done(Donate);
}

void FontMate::About::alreadyDonated() {
	done(AlreadyDonated);
}

void FontMate::About::donateLater() {
	done(DonateLater);
}
