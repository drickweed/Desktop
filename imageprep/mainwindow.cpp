#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "collectiontools.h"

/** QT headers **/
#include <QFileDialog>
#include <QDir>

/** JB headers **/
#include <jb/qt4/pathutils.h>

/** Application headers **/
#include "targetbutton.h"
#include "settingsdialog.h"
#include "settings.h"
#include "collectionnamedialog.h"

using namespace JB;
using namespace CollectionTools;

MainWindow::MainWindow(QWidget *parent):
		QMainWindow(parent),
		Ui::MainWindow(),
		last_sort_target("/"),
		last_image_source("/") {

		Ui::MainWindow::setupUi(this);
		setupModel();
		setupUi();
		loadExistingTargets();
		connectUi();
}

MainWindow::~MainWindow() {
	delete settings;
}

void MainWindow::setupModel() {
	settings = new Settings();

	m_document = new Document(this);

	images = new ImagesModel(this);
	images->setScale(100);

	editorScene = new ImageEditorScene(this);
}

void MainWindow::setupUi() {
	imagesSourceListView->setModel(images);
	thumbnailZoomSlider->setMaximum(images->maxScale());

	editorView->setScene(editorScene);

	imageViewDelegate = new ImageViewDelegate(imagesSourceListView);
	imagesSourceListView->setItemDelegate(imageViewDelegate);

	loadCropSettingsFromUi();

	sortButtonLayout = qobject_cast<QVBoxLayout*>(sortButtonsContainer->layout());

	backgroundImageView = new ImageViewWidget(this);
	QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(toolsDockWidgetContents->layout());
	layout->insertWidget(2, backgroundImageView);


}

void MainWindow::connectUi() {
	connect(imagePathBrowseButton, SIGNAL(clicked()), SLOT(showImagePathSelectDialog()));
	connect(thumbnailZoomSlider, SIGNAL(valueChanged(int)), images, SLOT(setScale(int)));

	connect(editorScene, SIGNAL(sceneRectChanged ( const QRectF &)), editorView, SLOT(updateSceneRect(const QRectF &)));
	connect(editorScene, SIGNAL(sceneRectChanged ( const QRectF &)), SLOT(centerEditorImage(const QRectF &)));
	connect(editorScene, SIGNAL(imageScaleChanged( const QRectF &)), editorView, SLOT(updateSceneRect(const QRectF &)));
	connect(editorScene, SIGNAL(imageScaleChanged(const QRectF &)), SLOT(centerEditorImage(QRectF)));
	connect(editorScene, SIGNAL(cropRectMoved(const QRectF &)), SLOT(updateCropRectPosition(const QRectF&)));

	connect(imagesSourceListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(loadImage(QModelIndex)));

	connect(heightCropSpinBox, SIGNAL(valueChanged(int)), editorScene, SLOT(setCropHeight(int)));
	connect(widthCropSpinBox, SIGNAL(valueChanged(int)), editorScene, SLOT(setCropWidth(int)));
	connect(topSpinBox, SIGNAL(valueChanged(int)), editorScene, SLOT(setCropTop(int)));
	connect(leftSpinBox, SIGNAL(valueChanged(int)), editorScene, SLOT(setCropLeft(int)));
	connect(imageScaleSlider, SIGNAL(valueChanged(int)), SLOT(updateImageScaleFromSlider(int)));

	connect(addSortTargetButton, SIGNAL(clicked()), SLOT(addSortTarget()));

	connect(setBackgroundButton, SIGNAL(clicked()), SLOT(setBackgroundImage()));
	connect(clearBackgroundButton, SIGNAL(clicked()), SLOT(clearBackgroundImage()));
	connect(settingsButton, SIGNAL(clicked()), SLOT(showSettingsDialog()));
}

void MainWindow::setBackgroundImage() {
	QString path = QFileDialog::getOpenFileName(this, tr("Select background image"), "/");

	if (path.isEmpty())
		return;

	setBackgroundImage(path);
}

void MainWindow::loadExistingTargets() {
	QDir collections = QDir(settings->collectionsPath(), "", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(QString collection, collections.entryList()) {
		addSortTarget(collection);
	}
}

void MainWindow::clearBackgroundImage() {
	editorScene->clearBackgroundImage();
	backgroundImageView->clearImage();
}

void MainWindow::setBackgroundImage(QString full_path) {
	editorScene->setBackgroundImage(full_path);
	backgroundImageView->setImage(full_path);
}

void MainWindow::updateCropRectPosition(const QRectF &new_crop_rect) {
	topSpinBox->setValue(new_crop_rect.top());
	leftSpinBox->setValue(new_crop_rect.left());
}

void MainWindow::showSettingsDialog() {
	SettingsDialog settingsDialog(settings);
	int result = settingsDialog.exec();

	switch (result) {
		case QDialog::Accepted:
			settingsDialog.storeInSettings();
			settings->sync();
			break;
		case QDialog::Rejected:
			settings->reload();
			break;
	}
}

void MainWindow::addSortTarget() {
	QString path = CollectionNameDialog::getName();

	if (path.isEmpty())
		return;

	last_sort_target = path;

	addSortTarget(path);
}

QString MainWindow::collectionPath(QString name) const {
	if (name.isEmpty())
		return "";

	QString full_path = Path::join(settings->collectionsPath(), name);
	Path::ensurePath(full_path);
	return full_path;
}

void MainWindow::processImageIntoTarget(QString collection_name) {
	bool write_succesfull = editorScene->currentTranslation().apply(editorScene->imagePath(), collectionPath(collection_name));

	if (write_succesfull) {
		buttons[collection_name]->incrementImageCount();
	} else {
		statusBar()->showMessage(QString("Error processing image to collection %1").arg(collection_name));
	}
}

void MainWindow::addSortTarget(QString collection_name) {
	TargetButton *button = new TargetButton(collection_name, this);
	button->setFtpInfo(settings->ftpHost(), settings->ftpUser(), settings->ftpPassword());
	button->setImageCount(dirImageCount(collectionPath(collection_name)));
	buttons[collection_name] = button;
	sortButtonLayout->insertWidget(sortButtonsContainer->children().count()-1, button);
	connect(button, SIGNAL(clicked(QString)), SLOT(processImageIntoTarget(QString)));
}

void MainWindow::loadCropSettingsFromUi() {
	editorScene->setCropPosition(QPoint(topSpinBox->value(), leftSpinBox->value()));
	editorScene->setCropSize(QSize(widthCropSpinBox->value(), heightCropSpinBox->value()));
}

void MainWindow::loadImage(QModelIndex index) {
	QString path = images->path(index);
	editorScene->setImage(path);
}

void MainWindow::centerEditorImage( const QRectF & rect) {
	editorView->fitInView(rect, Qt::KeepAspectRatio);
}

void MainWindow::showImagePathSelectDialog() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Select folder with images"), last_image_source, QFileDialog::ShowDirsOnly);

	if (path.isEmpty())
		return;

	loadImages(path);
}

void MainWindow::setTargetRemoveMode(bool state) {
	if (state) {

	} else {

	}
}

void MainWindow::loadImages(QString path) {
	last_image_source = path;
	images->populate(path);
}

void MainWindow::updateImageScaleFromSlider(int scale) {
	switch (currentImageScaler()) {
		case ScaleWidth:
			editorScene->setWidthScale(scale);
			break;
		case ScaleHeight:
			editorScene->setHeightScale(scale);
			break;
	}
}

MainWindow::ScaleDirection MainWindow::currentImageScaler() {
	return (ScaleDirection)scaleDirectionComboBox->currentIndex();
}

void MainWindow::updateImageScaleSlider(int scale_direction) {

}
