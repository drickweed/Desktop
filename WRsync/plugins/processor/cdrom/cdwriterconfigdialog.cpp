#include "cdwriterconfigdialog.h"

CDWriterConfigDialog::CDWriterConfigDialog(QWidget *parent) :
    QWidget(parent){
    setupUi(this);
}

void CDWriterConfigDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
