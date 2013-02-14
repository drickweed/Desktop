#pragma once

/** QT headers **/
#include <QWidget>

namespace osx {

    class MenuBar : public QObject {
        public:
            MenuBar(QObject *parent=0);

            void static generate(QWidget *parent);
    };

}
