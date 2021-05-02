//
// Created by weiwu on 21-4-30.
//

#include "RibbonMenu.h"
#include <QWidgetAction>

RibbonMenu::RibbonMenu(QWidget *parent) : QMenu(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

RibbonMenu::RibbonMenu(const QString &title, QWidget *parent) : QMenu(title, parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QAction *RibbonMenu::addRibbonMenu(RibbonMenu *menu) {
    return QMenu::addMenu(menu);
}

RibbonMenu *RibbonMenu::addRibbonMenu(const QString &title) {
    RibbonMenu *menu = new RibbonMenu(title, this);
    return menu;
}

RibbonMenu *RibbonMenu::addRibbonMenu(const QIcon &icon, const QString &title) {
    RibbonMenu *menu = new RibbonMenu(title, this);
    menu->setIcon(icon);
    return menu;
}

QAction *RibbonMenu::addWidget(QWidget *w) {
    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(w);
    addAction(action);
    return action;
}