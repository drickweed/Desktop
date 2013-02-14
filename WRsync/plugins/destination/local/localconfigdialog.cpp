#include "localconfigdialog.h"

LocalConfigDialog::LocalConfigDialog(QWidget *parent) :
    QWidget(parent){
    setupUi(this);
}

void LocalConfigDialog::changeEvent(QEvent *e)
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
