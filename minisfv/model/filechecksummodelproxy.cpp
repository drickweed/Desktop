#include "filechecksummodelproxy.h"

/** JB headers **/
#include <jb/std/bitops.h>

using namespace JB::BITOPS;

namespace MiniSFV {

	FileChecksumModelProxy::FileChecksumModelProxy(QObject *parent):
		QSortFilterProxyModel(parent),
		m_display_filter(0),
		model(NULL) {
	}

	void FileChecksumModelProxy::setSourceModel(QAbstractItemModel *sourceModel) {
		if (model) {
			disconnect(model, SIGNAL(calculatedChecksumSet(int)), this, SLOT(invalidate()));
		}

		if (sourceModel) {
			model = dynamic_cast<FileChecksumModel*>(sourceModel);
			if (model) {
				QSortFilterProxyModel::setSourceModel(sourceModel);
				connect(model, SIGNAL(calculatedChecksumSet(int)), SLOT(invalidate()));
				return;
			}
		} else
			model = NULL;
	}

	void FileChecksumModelProxy::changeFilter(uint bits, bool state) {
		setBit(m_display_filter, bits, state);
		invalidate();
	}

	void FileChecksumModelProxy::showOnlyErrors() {
		changeFilter(Checksum::Invalid | Checksum::Missing | Checksum::Unknown, true);
		changeFilter(Checksum::Valid, false);
	}

	void FileChecksumModelProxy::showAll() {
		changeFilter(Checksum::Invalid | Checksum::Missing | Checksum::Unknown | Checksum::Valid, false);
	}

	bool FileChecksumModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex& /* sourceParent */) const {

		// No active filter. return all records
		if (m_display_filter == 0)
			return true;

		const FileChecksumRecord record = model->getRecord(sourceRow);
		return (((isBitSet(Checksum::Valid, m_display_filter)) and (record.isValid())) or
			((isBitSet(Checksum::Invalid, m_display_filter)) and (record.isInvalid())) or
			((isBitSet(Checksum::Missing, m_display_filter)) and (not record.fileExists())) or
			((isBitSet(Checksum::Unknown, m_display_filter)) and (record.isUnknown())));

		return false;
	}

	void FileChecksumModelProxy::setDisplayFilter(uint filter) {
		m_display_filter = filter;
		invalidate();
	}
}
