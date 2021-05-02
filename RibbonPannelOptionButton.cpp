//
// Created by weiwu on 21-4-30.
//

#include "RibbonPannelOptionButton.h"
#include <QAction>

RibbonPannelOptionButton::RibbonPannelOptionButton(QWidget *parent)
        : QToolButton(parent) {
    setAutoRaise(true);
    setCheckable(false);
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setFixedSize(16, 16);
    setIconSize(QSize(10, 10));
    setIcon(QIcon("resource/ribbonPannelOptionButton.png"));
}

void RibbonPannelOptionButton::connectAction(QAction *action) {
    connect(this, &RibbonPannelOptionButton::clicked, action, &QAction::toggle);
}
