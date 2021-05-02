//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONPANNELOPTIONBUTTON_H
#define RIBBON_RIBBONPANNELOPTIONBUTTON_H


#include <QtWidgets/QToolButton>

class QAction;


class RibbonPannelOptionButton : public QToolButton {
Q_OBJECT
public:
    RibbonPannelOptionButton(QWidget *parent = Q_NULLPTR);

    void connectAction(QAction *action);
};


#endif //RIBBON_RIBBONPANNELOPTIONBUTTON_H
