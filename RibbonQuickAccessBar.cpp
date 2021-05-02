//
// Created by weiwu on 21-4-30.
//

#include "RibbonQuickAccessBar.h"
#include "RibbonButtonGroupWidget.h"


class RibbonQuickAccessBarPrivate {
public:
    RibbonButtonGroupWidget *groupWidget;
};

RibbonQuickAccessBar::RibbonQuickAccessBar(QWidget *parent)
        : RibbonCtrlContainer(nullptr, parent), m_d(new RibbonQuickAccessBarPrivate) {
    m_d->groupWidget = new RibbonButtonGroupWidget(this);
    setContainerWidget(m_d->groupWidget);
}


RibbonQuickAccessBar::~RibbonQuickAccessBar() {
    delete m_d;
}


void RibbonQuickAccessBar::addSeparator() {
    m_d->groupWidget->addSeparator();
}


void RibbonQuickAccessBar::addAction(QAction *act) {
    m_d->groupWidget->addAction(act);
}


void RibbonQuickAccessBar::addWidget(QWidget *w) {
    m_d->groupWidget->addWidget(w);
}


void RibbonQuickAccessBar::addMenu(QMenu *m, QToolButton::ToolButtonPopupMode popMode) {
    m_d->groupWidget->addMenu(m, popMode);
}


void RibbonQuickAccessBar::initStyleOption(QStyleOption *opt) {
    opt->initFrom(this);
}
