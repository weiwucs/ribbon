//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONQUICKACCESSBAR_H
#define RIBBON_RIBBONQUICKACCESSBAR_H

#include <QtCore/QArgument>
#include <QtWidgets/QAction>
#include <QtWidgets/QtWidgets>
#include "RibbonCtrlContainer.h"

class RibbonQuickAccessBarPrivate;

class RibbonQuickAccessBar : public RibbonCtrlContainer{
    Q_OBJECT
public:
    RibbonQuickAccessBar(QWidget *parent = 0);
    ~RibbonQuickAccessBar();
    void addSeparator();
    void addAction(QAction *act);
    void addWidget(QWidget *w);
    void addMenu(QMenu *m, QToolButton::ToolButtonPopupMode popMode = QToolButton::InstantPopup);

protected:
    virtual void initStyleOption(QStyleOption *opt);

private:
    RibbonQuickAccessBarPrivate *m_d;
};


#endif //RIBBON_RIBBONQUICKACCESSBAR_H
