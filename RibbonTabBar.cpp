//
// Created by weiwu on 21-5-1.
//

#include "RibbonTabBar.h"

RibbonTabBar::RibbonTabBar(QWidget *parent) : QTabBar(parent), m_tabMargin(6, 0, 0, 0) {
    setExpanding(false);
}


const QMargins &RibbonTabBar::tabMargin() const {
    return (m_tabMargin);
}


void RibbonTabBar::setTabMargin(const QMargins &tabMargin) {
    m_tabMargin = tabMargin;
}