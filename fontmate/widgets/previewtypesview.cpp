#include "previewtypesview.h"

PreviewTypesView::PreviewTypesView(QWidget *parent) :
    QWidget(parent){
    setupUi(this);
}

void PreviewTypesView::changeEvent(QEvent *e)
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
