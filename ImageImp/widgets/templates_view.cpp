#include "templates_view.h"

/** QT headers **/
#include <QMenu>
#include <QDebug>
#include <QSqlField>
#include <QSqlRecord>
#include <QVariant>

using namespace JB;

namespace ImageImp {

	TemplatesView::TemplatesView(QWidget * parent):
		QListView(parent) {
		setSelectionMode(QAbstractItemView::SingleSelection);

		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(contextMenu(const QPoint&)));
	}

	void TemplatesView::contextMenu(const QPoint &point) {
		QModelIndex index = indexAt(point);

		if (!index.isValid())
			return;

		QMenu menu;

		m_selected_index = index;

		QAction *action = menu.addAction(tr("Edit"));
		connect(action, SIGNAL(triggered()), SLOT(editTemplate()));

		menu.exec(viewport()->mapToGlobal(point));
	}

	void TemplatesView::editTemplate() {
		QString file_name = templatesModel()->fullPath(m_selected_index);
		qDebug() << "Open template for editing in photoshop" << file_name;
	}

	const QString TemplatesView::selectedTemplateName() const {
		QModelIndexList selection = selectedIndexes();

		if (selection.empty())
			return QString();

		return model()->data(selection[0]).toString();
	}

	void TemplatesView::selectTemplate(QString template_name) {

		QModelIndex template_index = QModelIndex();

		if (not template_name.isEmpty())
			template_index =templatesModel()->findByTemplateName(template_name);

		// Select first template if no valid template is given or found
		if (not template_index.isValid())
			template_index = templatesModel()->index(0,0);

		selectionModel()->select(template_index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
	}

	void TemplatesView::selectForAlbum(int album_id) {
		selectTemplate(Album(album_id).getTemplate(templatesModel()->templateType()).name());
	}

	void TemplatesView::saveSelection() const {
		if (not templatesModel()->hasValidAlbum())
			return;

		Album(templatesModel()->albumId()).setTemplate(templatesModel()->templateType(), selectedTemplateName());
	}

	TemplatesModel* TemplatesView::templatesModel() const {
		return dynamic_cast<TemplatesModel*>(model());
	}

	void TemplatesView::selectionChanged(const QItemSelection & /* selected */, const QItemSelection & /* deselected */) {
		saveSelection();
	}

	void TemplatesView::setModel(QAbstractItemModel *model) {
		if (this->model()) {
			disconnect(this->model(), SIGNAL(albumChanged(int)), this, SLOT(selectForAlbum(int)));
		}

		QListView::setModel(model);

		if (model) {
			connect(model, SIGNAL(albumChanged(int)), SLOT(selectForAlbum(int)));
		}
	}

	const QString TemplatesView::selectedTemplatePath() const {
		return templatesModel()->fullPath(selectedTemplate());
	}

	QModelIndex TemplatesView::selectedTemplate() const {
		QModelIndexList indexes = selectedIndexes();
		if (indexes.empty())
			return QModelIndex();
		else
			return indexes[0];
	}

}
