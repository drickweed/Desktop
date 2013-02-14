/** QT headers **/
#include <QString>
#include <QDebug>

/** Application headers **/
#include "summary.h"

/** JB headers **/
#include <jb/std/bitops.h>

using namespace JB::BITOPS;

namespace Checksum {
	enum DisplayFilterBits { Valid = 0x1, Invalid = 0x2, Missing = 0x4, Unchecked = 0x8 };
}

namespace MiniSFV {

	Summary::Summary(FileChecksumModel *model, QWidget *parent):
		QWidget(parent),
		model(model) {

		valid = new StatusSummaryItem(this);
		valid->setLabel(tr("Valid"));
		valid->hide();
		connect(valid, SIGNAL(stateChanged(int)), SLOT(updateDisplayFilter()));

		missing = new StatusSummaryItem(this);
		missing->setLabel(tr("Missing"));
		missing->hide();
		missing->setColor(QColor(127,255,212));
		connect(missing, SIGNAL(stateChanged(int)), SLOT(updateDisplayFilter()));

		invalid = new StatusSummaryItem(this);
		invalid->setLabel(tr("Invalid"));
		invalid->hide();
		invalid->setColor(QColor(255,150,150));
		connect(invalid, SIGNAL(stateChanged(int)), SLOT(updateDisplayFilter()));

		unknown = new StatusSummaryItem(this);
		unknown->setLabel(tr("Unchecked"));
		unknown->hide();
		unknown->setColor(QColor(Qt::white));
		connect(unknown, SIGNAL(stateChanged(int)), SLOT(updateDisplayFilter()));

		default_color = palette().color(QPalette::Button);

		valid_layout = new QVBoxLayout();
		valid_layout->addWidget(valid);

		invalid_layout = new QVBoxLayout();
		invalid_layout->addWidget(invalid);
		invalid_layout->addWidget(missing);
		invalid_layout->addWidget(unknown);

		top_layout = new QHBoxLayout();
		top_layout->addLayout(valid_layout);
		top_layout->addLayout(invalid_layout);

		setLayout(top_layout);
	}

	//TODO better i18n messages
	void Summary::updateValid(uint count, uint total) {
		if (count == total) {
			valid->setLabel(tr("All files valid"));
			valid->setColor(QColor(Qt::green));
			valid->useLabel();
		} else {
			valid->setLabel(tr("Valid"));
			valid->setColor(palette().color(QPalette::Window));
			valid->useCheckbox();
		}
		valid->setCount(count);
		valid->setVisible(count != 0);
	}

	void Summary::updateDisplayFilter() {
		uint filter = 0;
		setBit(filter, Checksum::Valid, valid->isChecked());
		setBit(filter, Checksum::Invalid, invalid->isChecked());
		setBit(filter, Checksum::Missing, missing->isChecked());
		setBit(filter, Checksum::Unchecked, unknown->isChecked());
		emit displayFilter(filter);
	}

	void Summary::updateInvalid(uint count) {
		invalid->setCount(count);
		invalid->setVisible(count != 0);
	}

	void Summary::updateMissing(uint count) {
		missing->setCount(count);
		missing->setVisible(count != 0);
	}

	void Summary::updateUnchecked(uint count) {
		unknown->setCount(count);
		unknown->setVisible(count != 0);
	}

	void Summary::update() {
		uint total_count = model->rowCount();
		updateValid(model->validCount(), total_count);
		updateInvalid(model->invalidCount());
		updateMissing(model->missingCount());
		updateUnchecked(model->uncheckedCount());
	}

	void Summary::checkOnlyErrorFilters() {
		valid->unSelect();
		invalid->select();
		unknown->select();
		missing->select();
	}

	void Summary::checkOnlyValidFilter() {
		valid->select();
		invalid->unSelect();
		unknown->unSelect();
		missing->unSelect();
	}
}
