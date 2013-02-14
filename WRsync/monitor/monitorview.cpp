#include "monitorview.h"

MonitorView::MonitorView(QWidget *parent) :
    QWidget(parent){
    setupUi(this);
}

void MonitorView::changeEvent(QEvent *e)
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
