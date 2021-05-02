//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONAPPLICATIONBUTTON_H
#define RIBBON_RIBBONAPPLICATIONBUTTON_H


#include <QtWidgets/QPushButton>

class RibbonApplicationButton : public QPushButton {
Q_OBJECT
public:
    RibbonApplicationButton(QWidget *parent = nullptr);

    RibbonApplicationButton(const QString &text, QWidget *parent = nullptr);

    RibbonApplicationButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
};


#endif //RIBBON_RIBBONAPPLICATIONBUTTON_H
