#include "uploaddialog.h"

UploadDialog::UploadDialog(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
}

void UploadDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
