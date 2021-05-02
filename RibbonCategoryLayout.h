//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONCATEGORYLAYOUT_H
#define RIBBON_RIBBONCATEGORYLAYOUT_H


#include <QtWidgets/QLayout>
#include "RibbonPannel.h"
#include "RibbonSeparatorWidget.h"
#include "RibbonCategory.h"

class RibbonCategoryLayoutItem;

class RibbonSeparatorWidget;

class RibbonCategoryLayoutPrivate;

class RibbonCategoryLayout : public QLayout {
Q_OBJECT
public:
    RibbonCategoryLayout(RibbonCategory *parent);

    ~RibbonCategoryLayout();

    RibbonCategory *ribbonCategory();

    virtual void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;

    virtual QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;

    virtual QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;

    RibbonCategoryLayoutItem *takePannelItem(int index);

    RibbonCategoryLayoutItem *takePannel(RibbonPannel *pannel);

    virtual int count() const Q_DECL_OVERRIDE;

    //追加一个pannel
    void addPannel(RibbonPannel *pannel);

    //插入一个pannel
    void insertPannel(int index, RibbonPannel *pannel);

    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;

    QSize sizeHint() const Q_DECL_OVERRIDE;

    QSize minimumSize() const Q_DECL_OVERRIDE;

    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    void invalidate() Q_DECL_OVERRIDE;

    //更新尺寸
    void updateGeometryArr();

    //执行位置调整
    void doLayout();

    //返回所有pannels
    QList<RibbonPannel *> pannels() const;

private slots:

    void onLeftScrollButtonClicked();

    void onRightScrollButtonClicked();

private:
    RibbonCategoryLayoutPrivate *m_d;
};

/**
 * RibbonCategoryLayoutItem 用于标识RibbonCategoryLayout的item
 */
class RibbonCategoryLayoutItem : public QWidgetItem {
public:
    RibbonCategoryLayoutItem(RibbonPannel *w);

    RibbonSeparatorWidget *separatorWidget;
    QRect mWillSetGeometry;
    QRect mWillSetSeparatorGeometry;
};

#endif //RIBBON_RIBBONCATEGORYLAYOUT_H
