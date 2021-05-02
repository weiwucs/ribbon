//
// Created by weiwu on 21-5-2.
//

#ifndef RIBBON_RIBBONCOMBOBOX_H
#define RIBBON_RIBBONCOMBOBOX_H


#include <QtCore/QMetaObject>
#include <QtWidgets/QComboBox>

class RibbonComboBox : public QComboBox{
    Q_OBJECT
public:
    RibbonComboBox(QWidget *parent = Q_NULLPTR);
};


#endif //RIBBON_RIBBONCOMBOBOX_H
