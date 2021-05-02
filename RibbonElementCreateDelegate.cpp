//
// Created by weiwu on 21-4-30.
//

#include "RibbonElementCreateDelegate.h"

RibbonStyleOption::RibbonStyleOption() {
    mainbarHeightOfficeStyleThreeRow = 160;
    mainbarHeightWPSStyleThreeRow = 130;
    mainbarHeightOfficeStyleTwoRow = 134;
    mainbarHeightWPSStyleTwoRow = 104;
    titleBarHight = 30;
    tabBarHight = 25;
    titleTextColor = Qt::black;
    widgetBord = QMargins(5, 1, 5, 5);
    tabBarBaseLineColor = QColor(186, 201, 219);
}


RibbonStyleOption::~RibbonStyleOption() {
}

RibbonElementCreateDelegate::RibbonElementCreateDelegate()
        : m_opt(new RibbonStyleOption()) {
}


RibbonElementCreateDelegate::~RibbonElementCreateDelegate() {
    if (m_opt) {
        delete m_opt;
    }
    m_opt = nullptr;
}

RibbonSeparatorWidget *RibbonElementCreateDelegate::createRibbonSeparatorWidget(int value, QWidget *parent) {
    return (new RibbonSeparatorWidget(value, parent));
}

RibbonSeparatorWidget *RibbonElementCreateDelegate::createRibbonSeparatorWidget(QWidget *parent) {
    return (new RibbonSeparatorWidget(parent));
}

RibbonGallery *RibbonElementCreateDelegate::createRibbonGallery(QWidget *parent) {
    return (new RibbonGallery(parent));
}

RibbonPannel *RibbonElementCreateDelegate::createRibbonPannel(QWidget *parent) {
    return (new RibbonPannel(parent));
}

RibbonGalleryGroup *RibbonElementCreateDelegate::createRibbonGalleryGroup(QWidget *parent) {
    return (new RibbonGalleryGroup(parent));
}

RibbonApplicationButton *RibbonElementCreateDelegate::createRibbonApplicationButton(QWidget *parent) {
    return (new RibbonApplicationButton(parent));
}

RibbonQuickAccessBar *RibbonElementCreateDelegate::createQuickAccessBar(QWidget *parent) {
    return (new RibbonQuickAccessBar(parent));
}

RibbonTabBar *RibbonElementCreateDelegate::createRibbonTabBar(QWidget *parent) {
    return (new RibbonTabBar(parent));
}

RibbonStackedWidget *RibbonElementCreateDelegate::createRibbonStackedWidget(RibbonBar *parent) {
    return (new RibbonStackedWidget(parent));
}

const RibbonStyleOption &RibbonElementCreateDelegate::getRibbonStyleOption() const {
    return (*m_opt);
}

RibbonCategory *RibbonElementCreateDelegate::createRibbonCategory(QWidget *parent) {
    return (new RibbonCategory(parent));
}

RibbonToolButton *RibbonElementCreateDelegate::createRibbonToolButton(QWidget *parent) {
    return (new RibbonToolButton(parent));
}

RibbonControlButton *RibbonElementCreateDelegate::createHidePannelButton(RibbonBar *parent) {
    RibbonControlButton *btn = new RibbonControlButton(parent);

    btn->setAutoRaise(false);
    btn->setObjectName("SARibbonBarHidePannelButton");
    btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn->setFixedSize(parent->tabBarHeight() - 4, parent->tabBarHeight() - 4);
    return (btn);
}

RibbonContextCategory *RibbonElementCreateDelegate::createRibbonContextCategory(QWidget *parent) {
    return (new RibbonContextCategory(parent));
}

RibbonButtonGroupWidget *RibbonElementCreateDelegate::craeteButtonGroupWidget(QWidget *parent) {
    return (new RibbonButtonGroupWidget(parent));
}

void RibbonElementCreateDelegate::setRibbonStyleOption(RibbonStyleOption *opt) {
    if (m_opt) {
        delete m_opt;
    }
    m_opt = opt;
}

RibbonPannelOptionButton *RibbonElementCreateDelegate::createRibbonPannelOptionButton(RibbonPannel *pannel) {
    return (new RibbonPannelOptionButton(pannel));
}