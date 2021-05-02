//
// Created by weiwu on 21-5-1.
//

#include "RibbonApplicationButton.h"


RibbonApplicationButton::RibbonApplicationButton(QWidget *parent)
        : QPushButton(parent) {
    setFocusPolicy(Qt::NoFocus);
    setFlat(true);
}


RibbonApplicationButton::RibbonApplicationButton(const QString &text, QWidget *parent)
        : QPushButton(text, parent) {
    setFocusPolicy(Qt::NoFocus);
    setFlat(true);
}


RibbonApplicationButton::RibbonApplicationButton(const QIcon &icon, const QString &text, QWidget *parent)
        : QPushButton(icon, text, parent) {
    setFocusPolicy(Qt::NoFocus);
    setFlat(true);
}
