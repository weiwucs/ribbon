//
// Created by weiwu on 21-5-2.
//

#ifndef RIBBON_RIBBONLINEEDIT_H
#define RIBBON_RIBBONLINEEDIT_H


#include <QtCore/QMetaObject>
#include <QtWidgets/QLineEdit>

class RibbonLineEdit : public QLineEdit{
    Q_OBJECT
public:
    RibbonLineEdit(QWidget *parent = Q_NULLPTR);
};


#endif //RIBBON_RIBBONLINEEDIT_H
