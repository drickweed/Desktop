#include "targetremovedialog.h"

TargetRemoveDialog::TargetRemoveDialog(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
}

void TargetRemoveDialog::changeEvent(QEvent *e)
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
