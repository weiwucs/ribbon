//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONELEMENTCREATEDELEGATE_H
#define RIBBON_RIBBONELEMENTCREATEDELEGATE_H

#include <QtGui/QColor>
#include <QtCore/QMargins>

#include <QWidget>
#include "RibbonGalleryGroup.h"
#include "RibbonSeparatorWidget.h"
#include "RibbonToolButton.h"
#include "RibbonGallery.h"
#include "RibbonPannelOptionButton.h"
#include "RibbonPannel.h"
#include "RibbonCategory.h"
#include "RibbonApplicationButton.h"
#include "RibbonQuickAccessBar.h"
#include "RibbonTabBar.h"
#include "RibbonStackedWidget.h"
#include "RibbonBar.h"
#include "RibbonControlButton.h"

class RibbonStyleOption{
public:
    RibbonStyleOption();
    virtual ~RibbonStyleOption();
    int mainbarHeightOfficeStyleThreeRow;   ///< OfficeStyle 样式下的mainbar高度
    int mainbarHeightWPSStyleThreeRow;      ///< WpsLiteStyle 样式下的mainbar高度
    int mainbarHeightOfficeStyleTwoRow;     ///< OfficeStyleTwoRow 样式下的mainbar高度
    int mainbarHeightWPSStyleTwoRow;        ///< WpsLiteStyleTwoRow 样式下的mainbar高度
    int titleBarHight;                      ///< 标题栏高度
    int tabBarHight;                        ///< ribbon tab 的高度
    QColor titleTextColor;                  ///< 标题颜色
    QMargins widgetBord;                    ///< 整个ribbonbar的四个边框，默认为0
    QColor tabBarBaseLineColor;             ///< tabbar 底部线条颜色
};

class RibbonElementCreateDelegate {

public:
    RibbonElementCreateDelegate();
    virtual ~RibbonElementCreateDelegate();

    virtual RibbonSeparatorWidget *createRibbonSeparatorWidget(int value, QWidget *parent);
    virtual RibbonSeparatorWidget *createRibbonSeparatorWidget(QWidget *parent);

    virtual RibbonPannel *createRibbonPannel(QWidget *parent);
    virtual RibbonToolButton *createRibbonToolButton(QWidget *parent);
    virtual RibbonGallery *createRibbonGallery(QWidget *parent);
    virtual RibbonGalleryGroup *createRibbonGalleryGroup(QWidget *parent);
    virtual RibbonCategory *createRibbonCategory(QWidget *parent);
    virtual RibbonApplicationButton*createRibbonApplicationButton(QWidget *parent);
    virtual RibbonQuickAccessBar *createQuickAccessBar(QWidget *parent);
    virtual RibbonTabBar *createRibbonTabBar(QWidget *parent);
    virtual RibbonStackedWidget *createRibbonStackedWidget(RibbonBar *parent);
    virtual RibbonContextCategory *createRibbonContextCategory(QWidget *parent);

    virtual RibbonControlButton *createHidePannelButton(RibbonBar *parent);
    virtual RibbonButtonGroupWidget *craeteButtonGroupWidget(QWidget *parent);
    virtual const RibbonStyleOption& getRibbonStyleOption() const;
    virtual void setRibbonStyleOption(RibbonStyleOption *opt);

    virtual RibbonPannelOptionButton *createRibbonPannelOptionButton(RibbonPannel *pannel);


private:
    RibbonStyleOption *m_opt;
};


#endif //RIBBON_RIBBONELEMENTCREATEDELEGATE_H
