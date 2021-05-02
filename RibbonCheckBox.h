//
// Created by weiwu on 21-5-2.
//

#ifndef RIBBON_RIBBONCHECKBOX_H
#define RIBBON_RIBBONCHECKBOX_H


#include <QtCore/QMetaObject>
#include <QtWidgets/QCheckBox>

class RibbonCheckBox : public QCheckBox{
    Q_OBJECT
public:
    RibbonCheckBox(QWidget *parent = Q_NULLPTR);
};


#endif //RIBBON_RIBBONCHECKBOX_H
