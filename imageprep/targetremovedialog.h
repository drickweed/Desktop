#pragma once

#include "ui_targetremovedialog.h"

class TargetRemoveDialog : public QDialog, private Ui::TargetRemoveDialog {
    Q_OBJECT
public:
    TargetRemoveDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};
