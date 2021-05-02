//
// Created by weiwu on 21-5-1.
//

#include <QDebug>
#include "RibbonCategoryLayout.h"
#include "RibbonElementManager.h"


class RibbonCategoryLayoutPrivate {
public:
    RibbonCategoryLayoutPrivate(RibbonCategoryLayout *par);

    //计算所有元素的sizehint总宽度
    int totalSizeHintWidth() const;

public:
    RibbonCategoryLayout *q_d;
    bool mDirty;
    QList<RibbonCategoryLayoutItem *> mItemList;
    QSize mSizeHint;
    RibbonCategoryScrollButton *mLeftScrollBtn;   ///< 在区域无法显示时显示的按钮
    RibbonCategoryScrollButton *mRightScrollBtn;  ///< 在区域无法显示时显示的按钮
    int mTotalWidth;
    int mXBase;
    bool mIsRightScrollBtnShow;     ///< 标记右滚动按钮是否需要显示
    bool mIsLeftScrollBtnShow;      ///< 标记左滚动按钮是否需要显示
};

//=============================================================
//RibbonCategoryLayoutItem
//=============================================================

RibbonCategoryLayoutItem::RibbonCategoryLayoutItem(RibbonPannel *w) : QWidgetItem(w) {
    separatorWidget = nullptr;
}


//=============================================================
//RibbonCategoryLayoutPrivate
//=============================================================

RibbonCategoryLayoutPrivate::RibbonCategoryLayoutPrivate(RibbonCategoryLayout *par)
        : q_d(par), mDirty(true), mSizeHint(50, 50), mLeftScrollBtn(nullptr), mRightScrollBtn(nullptr),
          mIsRightScrollBtnShow(false), mIsLeftScrollBtnShow(false) {
}


/**
 * @brief 计算所有元素的SizeHint宽度总和
 * @return
 */
int RibbonCategoryLayoutPrivate::totalSizeHintWidth() const {
    int total = 0;
    QMargins mag = q_d->contentsMargins();

    if (!mag.isNull()) {
        total += (mag.left() + mag.right());
    }
    //先计算总长
    for (RibbonCategoryLayoutItem *item : mItemList) {
        if (item->isEmpty()) {
            //如果是hide就直接跳过
            continue;
        }
        QSize pannelSize = item->widget()->sizeHint();
        QSize SeparatorSize(0, 0);
        if (item->separatorWidget) {
            SeparatorSize = item->separatorWidget->sizeHint();
        }
        total += pannelSize.width();
        total += SeparatorSize.width();
    }
    return (total);
}


//=============================================================
//RibbonCategoryLayout
//=============================================================

RibbonCategoryLayout::RibbonCategoryLayout(RibbonCategory *parent) : QLayout(parent),
                                                                     m_d(new RibbonCategoryLayoutPrivate(this)) {
    setContentsMargins(1, 1, 1, 1);
    m_d->mLeftScrollBtn = new RibbonCategoryScrollButton(Qt::LeftArrow, parent);
    m_d->mRightScrollBtn = new RibbonCategoryScrollButton(Qt::RightArrow, parent);
    m_d->mLeftScrollBtn->setVisible(false);
    m_d->mRightScrollBtn->setVisible(false);
    connect(m_d->mLeftScrollBtn, &QToolButton::clicked, this, &RibbonCategoryLayout::onLeftScrollButtonClicked);
    connect(m_d->mRightScrollBtn, &QToolButton::clicked, this, &RibbonCategoryLayout::onRightScrollButtonClicked);
}


RibbonCategoryLayout::~RibbonCategoryLayout() {
    QLayoutItem *item = Q_NULLPTR;

    while ((item = takeAt(0))) {
        delete item;
    }
    delete m_d;
}


RibbonCategory *RibbonCategoryLayout::ribbonCategory() {
    return (qobject_cast<RibbonCategory *>(parentWidget()));
}


void RibbonCategoryLayout::addItem(QLayoutItem *item) {
    Q_UNUSED(item);
    qWarning() << tr("in RibbonCategoryLayout cannot addItem,use addPannel instead");
    invalidate();
}


/**
 * @brief 返回pannel的layout
 * @param index 索引
 * @return
 * @note 注意，pannel是和分割线一起的，但这个只返回一个pannel对应的layout
 */
QLayoutItem *RibbonCategoryLayout::itemAt(int index) const {
    RibbonCategoryLayoutItem *item = m_d->mItemList.value(index, nullptr);

    return (item);
}


/**
 * @brief 提取layout
 * @param index
 * @return
 */
QLayoutItem *RibbonCategoryLayout::takeAt(int index) {
    return (takePannelItem(index));
}


RibbonCategoryLayoutItem *RibbonCategoryLayout::takePannelItem(int index) {
    if ((index >= 0) && (index < m_d->mItemList.size())) {
        invalidate();
        RibbonCategoryLayoutItem *item = m_d->mItemList.takeAt(index);
        if (item->widget()) {
            item->widget()->hide();
        }
        if (item->separatorWidget) {
            item->separatorWidget->hide();
        }
        return (item);
    }
    return (nullptr);
}


RibbonCategoryLayoutItem *RibbonCategoryLayout::takePannel(RibbonPannel *pannel) {
    for (int i = 0; i < m_d->mItemList.size(); ++i) {
        RibbonCategoryLayoutItem *item = m_d->mItemList[i];
        if (item->widget() == pannel) {
            return (takePannelItem(i));
        }
    }
    return (nullptr);
}


int RibbonCategoryLayout::count() const {
    return (m_d->mItemList.size());
}


void RibbonCategoryLayout::addPannel(RibbonPannel *pannel) {
    insertPannel(m_d->mItemList.count(), pannel);
}


/**
 * @brief 插入一个pannel
 * @param index 索引
 * @param pannel
 * @return 返回对应的分割线RibbonSeparatorWidget
 * @note 在RibbonCategoryLayout的布局中，一个pannel会携带一个分割线
 */
void RibbonCategoryLayout::insertPannel(int index, RibbonPannel *pannel) {
    index = qMax(0, index);
    index = qMin(m_d->mItemList.count(), index);
    RibbonCategoryLayoutItem *item = new RibbonCategoryLayoutItem(pannel);

    //分割线
    item->separatorWidget = RibbonSubElementDelegate->createRibbonSeparatorWidget(parentWidget());
    //插入list中
    m_d->mItemList.insert(index, item);
    //标记需要重新计算尺寸
    invalidate();
}


QSize RibbonCategoryLayout::sizeHint() const {
    return (m_d->mSizeHint);
}


QSize RibbonCategoryLayout::minimumSize() const {
    return (m_d->mSizeHint);
}


/**
 * @brief RibbonCategory充满整个stacked widget
 * @return
 */
Qt::Orientations RibbonCategoryLayout::expandingDirections() const {
    return (Qt::Horizontal | Qt::Vertical);
}


void RibbonCategoryLayout::invalidate() {
    m_d->mDirty = true;
    QLayout::invalidate();
}


/**
 * @brief 更新尺寸
 */
void RibbonCategoryLayout::updateGeometryArr() {
    RibbonCategory *category = qobject_cast<RibbonCategory *>(parentWidget());

    int categoryWidth = category->width();
    QMargins mag = contentsMargins();
    int height = category->height();
    int y = 0;

    if (!mag.isNull()) {
        y = mag.top();
        height -= (mag.top() + mag.bottom());
        categoryWidth -= (mag.right() + mag.left());
    }
    //total 是总宽，不是x坐标系，x才是坐标系
    int total = m_d->totalSizeHintWidth();
    //记录可以扩展的数量
    int canExpandingCount = 0;
    //扩展的宽度
    int expandWidth = 0;

    //如果total < categoryWidth,m_d->mXBase可以设置为0
    //判断是否超过总长度
    if (total > categoryWidth) {
        //超过总长度，需要显示滚动按钮
        if (0 == m_d->mXBase) {
            //已经移动到最左，需要可以向右移动
            m_d->mIsRightScrollBtnShow = true;
            m_d->mIsLeftScrollBtnShow = false;
        } else if (m_d->mXBase <= (categoryWidth - total)) {
            //已经移动到最右，需要可以向左移动
            m_d->mIsRightScrollBtnShow = false;
            m_d->mIsLeftScrollBtnShow = true;
        } else {
            //移动到中间两边都可以动
            m_d->mIsRightScrollBtnShow = true;
            m_d->mIsLeftScrollBtnShow = true;
        }
    } else {
        //说明total 小于 categoryWidth
        m_d->mIsRightScrollBtnShow = false;
        m_d->mIsLeftScrollBtnShow = false;
        //这个是避免一开始totalWidth > categorySize.width()，通过滚动按钮调整了m_d->mBaseX
        //随之调整了窗体尺寸，调整后totalWidth < categorySize.width()导致category在原来位置
        //无法显示，必须这里把mBaseX设置为0
        m_d->mXBase = 0;
        //

        for (RibbonCategoryLayoutItem *item : m_d->mItemList) {
            if (RibbonPannel *p = qobject_cast<RibbonPannel *>(item->widget())) {
                if (p->isExpanding()) {
                    //pannel可扩展
                    ++canExpandingCount;
                }
            }
        }
        //计算可扩展的宽度
        if (canExpandingCount > 0) {
            expandWidth = (categoryWidth - total) / canExpandingCount;
        } else {
            expandWidth = 0;
        }
    }
    total = 0;// total重新计算
    int x = m_d->mXBase;

    //先按照sizeHint设置所有的尺寸
    for (RibbonCategoryLayoutItem *item : m_d->mItemList) {
        if (item->isEmpty()) {
            //如果是hide就直接跳过
            if (item->separatorWidget) {
                //pannel hide分割线也要hide
                item->separatorWidget->hide();
            }
            item->mWillSetGeometry = QRect(0, 0, 0, 0);
            item->mWillSetSeparatorGeometry = QRect(0, 0, 0, 0);
            continue;
        }
        RibbonPannel *p = qobject_cast<RibbonPannel *>(item->widget());
        if (nullptr == p) {
            qDebug() << "unknow widget in RibbonCategoryLayout";
            continue;
        }
        QSize pannelSize = p->sizeHint();
        QSize SeparatorSize(0, 0);
        if (item->separatorWidget) {
            SeparatorSize = item->separatorWidget->sizeHint();
        }
        if (p->isExpanding()) {
            //可扩展，就把pannel扩展到最大
            pannelSize.setWidth(pannelSize.width() + expandWidth);
        }
        int w = pannelSize.width();
        item->mWillSetGeometry = QRect(x, y, w, height);
        x += w;
        total += w;
        w = SeparatorSize.width();
        item->mWillSetSeparatorGeometry = QRect(x, y, w, height);
        x += w;
        total += w;
    }
    m_d->mTotalWidth = total;
    QWidget *cp = category->parentWidget();
    int parentHeight = (nullptr == cp) ? height : cp->height();
    int parentWidth = (nullptr == cp) ? total : cp->width();
    m_d->mSizeHint = QSize(parentWidth, parentHeight);
}


/**
 * @brief 执行layout调整
 */
void RibbonCategoryLayout::doLayout() {
    if (m_d->mDirty) {
        updateGeometryArr();
    }
    RibbonCategory *category = qobject_cast<RibbonCategory *>(parentWidget());

    //两个滚动按钮的位置永远不变
    m_d->mLeftScrollBtn->setGeometry(0, 0, 12, category->height());
    m_d->mRightScrollBtn->setGeometry(category->width() - 12, 0, 12, category->height());
    QList<QWidget *> showWidgets, hideWidgets;

    for (RibbonCategoryLayoutItem *item:m_d->mItemList) {
        if (item->isEmpty()) {
            hideWidgets << item->widget();
            if (item->separatorWidget) {
                hideWidgets << item->separatorWidget;
            }
        } else {
            item->widget()->setFixedSize(item->mWillSetGeometry.size());
            item->widget()->move(item->mWillSetGeometry.topLeft());
//            item->setGeometry(item->mWillSetGeometry);
            showWidgets << item->widget();
            if (item->separatorWidget) {
                item->separatorWidget->setGeometry(item->mWillSetSeparatorGeometry);
                showWidgets << item->separatorWidget;
            }
        }
    }


    m_d->mRightScrollBtn->setVisible(m_d->mIsRightScrollBtnShow);
    m_d->mLeftScrollBtn->setVisible(m_d->mIsLeftScrollBtnShow);
    if (m_d->mIsRightScrollBtnShow) {
        m_d->mRightScrollBtn->raise();
    }
    if (m_d->mIsLeftScrollBtnShow) {
        m_d->mLeftScrollBtn->raise();
    }
    // 不在上面那里进行show和hide因为这会触发RibbonPannelLayout的重绘，导致循环绘制，非常影响效率
    for (QWidget *w : showWidgets) {
        w->show();
    }
    for (QWidget *w : hideWidgets) {
        w->hide();
    }
}


/**
 * @brief 返回所有pannels
 * @return
 */
QList<RibbonPannel *> RibbonCategoryLayout::pannels() const {
    QList<RibbonPannel *> res;

    for (RibbonCategoryLayoutItem *item : m_d->mItemList) {
        RibbonPannel *p = qobject_cast<RibbonPannel *>(item->widget());
        res.append(p);
    }
    return (res);
}


void RibbonCategoryLayout::onLeftScrollButtonClicked() {
    RibbonCategory *category = qobject_cast<RibbonCategory *>(parentWidget());
    int width = category->width();
    //求总宽
    int totalWidth = m_d->mTotalWidth;

    if (totalWidth > width) {
        int tmp = m_d->mXBase + width;
        if (tmp > 0) {
            tmp = 0;
        }
        m_d->mXBase = tmp;
    } else {
        m_d->mXBase = 0;
    }
    invalidate();
}


void RibbonCategoryLayout::onRightScrollButtonClicked() {
    RibbonCategory *category = qobject_cast<RibbonCategory *>(parentWidget());
    int width = category->width();
    //求总宽
    int totalWidth = m_d->mTotalWidth;

    if (totalWidth > width) {
        int tmp = m_d->mXBase - width;
        if (tmp < (width - totalWidth)) {
            tmp = width - totalWidth;
        }
        m_d->mXBase = tmp;
    } else {
        m_d->mXBase = 0;
    }
    invalidate();
}


void RibbonCategoryLayout::setGeometry(const QRect &rect) {
    m_d->mDirty = false;
    updateGeometryArr();
    QLayout::setGeometry(rect);
    doLayout();
}


