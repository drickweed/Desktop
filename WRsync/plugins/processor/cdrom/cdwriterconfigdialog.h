#ifndef CDWRITERCONFIGDIALOG_H
#define CDWRITERCONFIGDIALOG_H

#include "ui_cdwriterconfigdialog.h"

class CDWriterConfigDialog : public QWidget, private Ui::CDWriterConfigDialog {
    Q_OBJECT
public:
    CDWriterConfigDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // CDWRITERCONFIGDIALOG_H
