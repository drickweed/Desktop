#pragma once

/** QT headers **/
#include <QMainWindow>
#include <QList>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMap>

/** JB headers **/
#include <jb/image/imagesmodel.h>
#include <jb/image/imagedelegate.h>
#include <jb/image/imageviewwidget.h>

/** Application headers **/
#include "ui_mainwindow.h"
#include "imageeditorscene.h"
#include "document.h"
#include "targetbutton.h"
#include "settings.h"

using namespace JB;

class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT

	public:
		enum ScaleDirection { ScaleWidth, ScaleHeight };

		MainWindow(QWidget *parent = 0);
		~MainWindow();

		Document* document() const { return m_document; }

		void loadImages(QString path);

	private slots:
		void addSortTarget();
		void setTargetRemoveMode(bool state);
		void showImagePathSelectDialog();

		void centerEditorImage( const QRectF & rect);
		void loadImage(QModelIndex index);

		void processImageIntoTarget(QString target_full_path);
		void updateImageScaleFromSlider(int scale);
		ScaleDirection currentImageScaler();
		void updateImageScaleSlider(int scale_direction);
		void setBackgroundImage();
		void setBackgroundImage(QString full_path);
		void clearBackgroundImage();
		void updateCropRectPosition(const QRectF &new_crop_rect);
		void showSettingsDialog();

	private:
		void connectUi();
		void setupModel();
		void setupUi();

		void loadCropSettingsFromUi();

		void addSortTarget(QString full_path);
		void removeSortTarget(QString full_path);
		void loadExistingTargets();

		QString collectionPath(QString name) const;

		QStringList sortTargets;

		ImagesModel *images;
		ImageEditorScene *editorScene;
		ImageViewDelegate *imageViewDelegate;
		ImageViewWidget *backgroundImageView;

		Document *m_document;
		Settings *settings;

		QString last_sort_target,
				last_image_source;

		QMap<QString, TargetButton*> buttons;

		QVBoxLayout *sortButtonLayout;
};

