#pragma once
#include "ui_uploaddialog.h"

class UploadDialog : public QDialog, private Ui::UploadDialog {
    Q_OBJECT
public:
    UploadDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};
