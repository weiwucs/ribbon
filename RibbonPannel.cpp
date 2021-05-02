//
// Created by weiwu on 21-4-30.
//

#include <QtWidgets/QWidgetAction>
#include <QMenu>
#include <QApplication>
#include <QtGui/QPainter>
#include "RibbonPannel.h"
#include "RibbonPannelOptionButton.h"
#include "RibbonElementManager.h"


const int c_higherModehight = 98;
const int c_lowerModehight = 72;
const int c_iconHighForHigerLarge = 32;
const QSize c_iconSizeForHigerLarge = QSize(c_iconHighForHigerLarge, c_iconHighForHigerLarge);
const int c_iconHighForHigerSmall = 16;
const QSize c_iconSizeForHigerSmall = QSize(c_iconHighForHigerSmall, c_iconHighForHigerSmall);

class RibbonPannelPrivate {
public:
    RibbonPannelPrivate(RibbonPannel *p);

    //根据m_pannelLayoutMode返回gridLayout应该增加的行数
    int rowadded();

    void createLayout();

    //返回最后一个添加的action对应的button，前提是最后一个是toolbutton，否则返回nullptr
    RibbonToolButton *lastAddActionButton();

    RibbonPannelItem::RowProportion m_lastRp; ///< 记录addAction等函数设置的rp，用于actionEvent添加
    RibbonPannel *Parent;
    RibbonPannelLayout *m_layout;
    QPoint m_nextElementPosition;
    int m_row;                                              ///< 记录小action所在的gridLayout行数，gridLayout总共划分为6行，用于满足3行或2行的按钮需求
    RibbonPannelOptionButton *m_optionActionButton;
    RibbonPannel::PannelLayoutMode m_pannelLayoutMode;    ///< pannel的布局模式，默认为3行模式ThreeRowMode
    bool m_isCanCustomize;                                  ///< 记录是否可自定义
};

RibbonPannelPrivate::RibbonPannelPrivate(RibbonPannel *p)
        : Parent(p), m_layout(nullptr), m_nextElementPosition(3, 3), m_row(0), m_optionActionButton(nullptr),
          m_pannelLayoutMode(RibbonPannel::ThreeRowMode), m_isCanCustomize(true) {
    createLayout();
}

int RibbonPannelPrivate::rowadded() {
    switch (m_pannelLayoutMode) {
        case RibbonPannel::ThreeRowMode:
            return (2);

        case RibbonPannel::TwoRowMode:
            return (3);

        default:
            break;
    }
    return (2);
}

void RibbonPannelPrivate::createLayout() {
    m_layout = new RibbonPannelLayout(Parent);
    m_layout->setSpacing(2);
    m_layout->setContentsMargins(2, 2, 2, 2);
}

RibbonToolButton *RibbonPannelPrivate::lastAddActionButton() {
    QWidget *w = m_layout->lastWidget();

    return (qobject_cast<RibbonToolButton *>(w));
}

RibbonPannelItem::RibbonPannelItem(QWidget *widget) : QWidgetItem(widget), action(nullptr), customWidget(false) {
}


bool RibbonPannelItem::isEmpty() const {
    return (action == 0 || !action->isVisible());
}

RibbonPannelLayout::RibbonPannelLayout(QWidget *p) : QLayout(p), m_columnCount(0), m_expandFlag(false), m_dirty(true) {
    setSpacing(1);
    RibbonPannel *tb = qobject_cast<RibbonPannel *>(p);

    if (!tb) {
        return;
    }
}


RibbonPannelLayout::~RibbonPannelLayout() {
    while (!m_items.isEmpty()) {
        RibbonPannelItem *item = m_items.takeFirst();
        if (QWidgetAction * widgetAction = qobject_cast<QWidgetAction *>(item->action)) {
            if (item->customWidget) {
                widgetAction->releaseWidget(item->widget());
            }
        }
        delete item;
    }
}


/**
 * @brief 通过action查找索引，用于actionEvent添加action用
 * @param action
 * @return 没有查到返回-1
 */
int RibbonPannelLayout::indexOf(QAction *action) const {
    for (int i = 0; i < m_items.count(); ++i) {
        if (m_items.at(i)->action == action) {
            return (i);
        }
    }
    return (-1);
}


void RibbonPannelLayout::addItem(QLayoutItem *item) {
    Q_UNUSED(item);
    qWarning("RibbonPannelLayout::addItem(): please use addAction() instead");
    return;
}


/**
 * @brief RibbonPannel主要通过此函数来添加action
 * @param act
 * @param rp 布局策略
 */
void RibbonPannelLayout::insertAction(int index, QAction *act, RibbonPannelItem::RowProportion rp) {
    index = qMax(0, index);
    index = qMin(m_items.count(), index);
    RibbonPannelItem *item = createItem(act, rp);

    if (item) {
        m_items.insert(index, item);
        //标记需要重新计算尺寸
        invalidate();
    }
}


QLayoutItem *RibbonPannelLayout::itemAt(int index) const {
    if ((index < 0) || (index >= m_items.count())) {
        return (nullptr);
    }
    return (m_items.at(index));
}


QLayoutItem *RibbonPannelLayout::takeAt(int index) {
    if ((index < 0) || (index >= m_items.count())) {
        return (nullptr);
    }
    RibbonPannelItem *item = m_items.takeAt(index);

    QWidgetAction *widgetAction = qobject_cast<QWidgetAction *>(item->action);

    if ((widgetAction != 0) && item->customWidget) {
        widgetAction->releaseWidget(item->widget());
    } else {
        // destroy the QToolButton/QToolBarSeparator
        item->widget()->hide();
        item->widget()->deleteLater();
    }

    invalidate();
    return (item);
}


int RibbonPannelLayout::count() const {
    return (m_items.count());
}


bool RibbonPannelLayout::isEmpty() const {
    return (m_items.isEmpty());
}


void RibbonPannelLayout::invalidate() {
    m_dirty = true;
    QLayout::invalidate();
}


Qt::Orientations RibbonPannelLayout::expandingDirections() const {
    return (Qt::Horizontal);
}


QSize RibbonPannelLayout::minimumSize() const {
    return (m_sizeHint);
}


QSize RibbonPannelLayout::sizeHint() const {
    return (m_sizeHint);
}


/**
 * @brief 通过action获取RibbonPannelItem
 * @param action
 * @return 如果没有返回nullptr
 */
RibbonPannelItem *RibbonPannelLayout::pannelItem(QAction *action) const {
    int index = indexOf(action);

    if (index >= 0) {
        return (m_items[index]);
    }
    return (nullptr);
}


/**
 * @brief 获取最后一个添加的item
 * @return 如果没有返回nullptr
 */
RibbonPannelItem *RibbonPannelLayout::lastItem() const {
    if (m_items.isEmpty()) {
        return (nullptr);
    }
    return (m_items.last());
}


/**
 * @brief 获取最后生成的窗口
 * @return 如果无窗口或者item为空，返回nullptr
 */
QWidget *RibbonPannelLayout::lastWidget() const {
    RibbonPannelItem *item = lastItem();

    if (item) {
        return (item->widget());
    }
    return (nullptr);
}


/**
 * @brief 根据pannel的默认参数得到的pannel高度
 * @return
 */
int RibbonPannelLayout::defaultPannelHeight() const {
    RibbonPannel *pannel = qobject_cast<RibbonPannel *>(parentWidget());
    int high = c_higherModehight;

    if (pannel) {
        switch (pannel->pannelLayoutMode()) {
            case RibbonPannel::ThreeRowMode:
                high = c_higherModehight;
                break;

            case RibbonPannel::TwoRowMode:
                high = c_lowerModehight;
                break;

            default:
                high = c_higherModehight;
                break;
        }
    }
    return (high);
}


/**
 * @brief 移动两个item
 * @param from
 * @param to
 * @note 移动完后所有都失效，需要重新布局
 */
void RibbonPannelLayout::move(int from, int to) {
    if (from == to) {
        return;
    }
    if (to < 0) {
        to = 0;
    }
    if (to >= count()) {
        to = count() - 1;
    }
    m_items.move(from, to);
    invalidate();
}


/**
 * @brief 判断是否需要重新布局
 * @return
 */
bool RibbonPannelLayout::isDirty() const {
    return (m_dirty);
}


/**
 * @brief 布局所有action
 */
void RibbonPannelLayout::layoutActions() {
    if (m_dirty) {
        updateGeomArray(geometry());
    }
    QList<QWidget *> showWidgets, hideWidgets;

    for (RibbonPannelItem *item:m_items) {
        if (item->isEmpty()) {
            hideWidgets << item->widget();
        } else {
            item->setGeometry(item->itemWillSetGeometry);
//            item->widget()->setFixedSize(item->itemWillSetGeometry.size());
//            item->widget()->move(item->itemWillSetGeometry.topLeft());
            showWidgets << item->widget();
        }
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
 * @brief 把action转换为item
 *
 * 此函数参考QToolBarItem *QToolBarLayout::createItem(QAction *action)
 *
 * 对于普通QAction，此函数会创建RibbonToolButton，RibbonToolButton的类型参考RibbonPannelItem::RowProportion，
 * @param action
 * @param rp 行高占比情况
 * @return 转换的RibbonPannelItem
 * @note 每个RibbonPannelItem最终都会携带一个widget，传入的是QWidgetAction的话，会直接使用QWidgetAction带的widget，
 * 否则会内部生成一个RibbonToolButton
 *
 */
RibbonPannelItem *RibbonPannelLayout::createItem(QAction *action, RibbonPannelItem::RowProportion rp) {
    bool customWidget = false;
    QWidget *widget = nullptr;
    RibbonPannel *pannel = qobject_cast<RibbonPannel *>(parentWidget());

    if (!pannel) {
        return (nullptr);
    }
    if (QWidgetAction * widgetAction = qobject_cast<QWidgetAction *>(action)) {
        widget = widgetAction->requestWidget(pannel);
        if (widget != nullptr) {
            widget->setAttribute(Qt::WA_LayoutUsesWidgetRect);
            customWidget = true;//标记为true，在移除的时候是不会对这个窗口进行删除，false默认会进行删除如RibbonSeparatorWidget和RibbonToolButton
        }
    } else if (action->isSeparator()) {
        RibbonSeparatorWidget *sep = RibbonSubElementDelegate->createRibbonSeparatorWidget(pannel);
        widget = sep;
    }
    //不是widget，自动生成RibbonToolbutton
    if (!widget) {
        RibbonToolButton::RibbonButtonType buttonType =
                ((rp == RibbonPannelItem::Large) ? RibbonToolButton::LargeButton
                                                 : RibbonToolButton::SmallButton);

        RibbonToolButton *button = RibbonSubElementDelegate->createRibbonToolButton(pannel);
        button->setAutoRaise(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setButtonType(buttonType);
        if (RibbonToolButton::LargeButton == buttonType) {
            //根据pannel的模式设置button样式
            button->setLargeButtonType((pannel->isTwoRow())
                                       ? RibbonToolButton::Lite
                                       : RibbonToolButton::Normal);
        }
        button->setDefaultAction(action);
        //根据QAction的属性设置按钮的大小

        QObject::connect(button, &RibbonToolButton::triggered, pannel, &RibbonPannel::actionTriggered);
        widget = button;
    }
    //这时总会有widget
    widget->hide();
    RibbonPannelItem *result = new RibbonPannelItem(widget);

    result->rowProportion = rp;
    result->customWidget = customWidget;
    result->action = action;
    return (result);
}


/**
 * @brief 更新尺寸
 */
void RibbonPannelLayout::updateGeomArray(const QRect &setrect) {
    RibbonPannel *pannel = qobject_cast<RibbonPannel *>(parentWidget());

    if (!pannel) {
        return;
    }
//    QWidget *pannelPar = pannel->parentWidget();
//    if(pannelPar)
//    defaultPannelHeight();
    int height = setrect.height();
    const QMargins mag = this->contentsMargins();
    const int spacing = this->spacing();
    int x = mag.left();
    //获取pannel的布局模式 3行或者2行
    //rowcount 是ribbon的行，有2行和3行两种
    const short rowCount = (pannel->pannelLayoutMode() == RibbonPannel::ThreeRowMode) ? 3 : 2;
    // largeHeight是对应large占比的高度
    const int largeHeight = height - mag.top() - mag.bottom() - pannel->titleHeight();

    m_largeHeight = largeHeight;
    //计算smallHeight的高度
    const int smallHeight = (largeHeight - (rowCount - 1) * spacing) / rowCount;
    //Medium行的y位置
    const int yMediumRow0 = (2 == rowCount) ? mag.top()
                                            : (mag.top() + ((largeHeight - 2 * smallHeight) / 3));
    const int yMediumRow1 = (2 == rowCount) ? (mag.top() + smallHeight + spacing)
                                            : (mag.top() + ((largeHeight - 2 * smallHeight) / 3) * 2 + smallHeight);
    //Small行的y位置
    const int ySmallRow0 = mag.top();
    const int ySmallRow1 = mag.top() + smallHeight + spacing;
    const int ySmallRow2 = mag.top() + 2 * (smallHeight + spacing);
    //row用于记录下个item应该属于第几行，item->rowIndex用于记录当前处于第几行，
    //item->rowIndex主要用于RibbonPannelItem::Medium
    short row = 0;
    int column = 0;
    //记录每列最大的宽度
    int columMaxWidth = 0;
    //记录总宽度
    int totalWidth = 0;

    int itemCount = m_items.count();

    for (int i = 0; i < itemCount; ++i) {
        RibbonPannelItem *item = m_items.at(i);
        if (item->isEmpty()) {
            //如果是hide就直接跳过
            item->rowIndex = -1;
            item->columnIndex = -1;
            continue;
        }

        QSize hint = item->sizeHint();
        Qt::Orientations exp = item->expandingDirections();
        if (item->widget()) {
            //有窗口是水平扩展，则标记为扩展
            if ((item->widget()->sizePolicy().horizontalPolicy() & QSizePolicy::ExpandFlag)) {
                m_expandFlag = true;
            }
        }
        RibbonPannelItem::RowProportion rp = item->rowProportion;
        if (RibbonPannelItem::None == rp) {
            //为定义行占比但是垂直扩展，就定义为Large占比，否则就是small占比
            if (exp & Qt::Vertical) {
                rp = RibbonPannelItem::Large;
            } else {
                rp = RibbonPannelItem::Small;
            }
        }
        //开始根据占比和layoutmode来布局
        switch (rp) {
            case RibbonPannelItem::Large: {
                // ！！在Large，如果不是处于新列的第一行，就需要进行换列处理
                // 把large一直设置在下一列的开始
                if (row != 0) {
                    x += (columMaxWidth + spacing);
                    ++column;
                    row = 0;
                    columMaxWidth = 0;
                }
                //
                item->rowIndex = 0;
                item->columnIndex = column;
                item->itemWillSetGeometry = QRect(x, mag.top(), hint.width(), largeHeight);
                columMaxWidth = hint.width();
                //换列，x自动递增到下个坐标，列数增加，行数归零，最大列宽归零
                x += (columMaxWidth + spacing);
                row = 0;
                columMaxWidth = 0;
                ++column;
            }
                break;

            case RibbonPannelItem::Medium: {
                //2行模式下Medium和small等价
                if (2 == rowCount) {
                    if (0 == row) {
                        item->rowIndex = 0;
                        item->columnIndex = column;
                        item->itemWillSetGeometry = QRect(x, yMediumRow0, hint.width(), smallHeight);
                        columMaxWidth = hint.width();
                        //下个row为1
                        row = 1;
                        //x不变
                    } else {
                        item->rowIndex = 1;
                        item->columnIndex = column;
                        item->itemWillSetGeometry = QRect(x, yMediumRow1, hint.width(), smallHeight);
                        //和上个进行比较得到最长宽度
                        columMaxWidth = qMax(columMaxWidth, hint.width());
                        //换列，x自动递增到下个坐标，列数增加，行数归零，最大列宽归零
                        x += (columMaxWidth + spacing);
                        row = 0;
                        columMaxWidth = 0;
                        ++column;
                    }
                } else {
                    //3行模式
                    if (0 == row) {
                        item->rowIndex = 0;
                        item->columnIndex = column;
                        item->itemWillSetGeometry = QRect(x, yMediumRow0, hint.width(), smallHeight);
                        columMaxWidth = hint.width();
                        row = 1;
                        //x不变
                    } else if (1 == row) {
                        item->rowIndex = 1;
                        item->columnIndex = column;
                        item->itemWillSetGeometry = QRect(x, yMediumRow1, hint.width(), smallHeight);
                        columMaxWidth = qMax(columMaxWidth, hint.width());
                        //换列，x自动递增到下个坐标，列数增加，行数归零，最大列宽归零
                        x += (columMaxWidth + spacing);
                        row = 0;
                        columMaxWidth = 0;
                        ++column;
                    } else {
                        //这种模式一般情况会发生在当前列前两行是Small，添加了一个Medium
                        //这时需要先换列
                        //换列，x自动递增到下个坐标，列数增加，行数归零，最大列宽归零
                        x += (columMaxWidth + spacing);
                        ++column;
                        row = 0;
                        columMaxWidth = 0;
                        //换列后此时等价于0 == row
                        item->rowIndex = 0;
                        item->columnIndex = column;
                        item->itemWillSetGeometry = QRect(x, yMediumRow0, hint.width(), smallHeight);
                        columMaxWidth = hint.width();
                        row = 1;
                    }
                }
            }
                break;

            case RibbonPannelItem::Small: {
                if (0 == row) {
                    //第一行
                    item->rowIndex = 0;
                    item->columnIndex = column;
                    item->itemWillSetGeometry = QRect(x, ySmallRow0, hint.width(), smallHeight);
                    columMaxWidth = hint.width();
                    //下个row为1
                    row = 1;
                    //x不变
                } else if (1 == row) {
                    //第二行
                    item->rowIndex = 1;
                    item->columnIndex = column;
                    item->itemWillSetGeometry = QRect(x, ySmallRow1, hint.width(), smallHeight);
                    //和上个进行比较得到最长宽度
                    columMaxWidth = qMax(columMaxWidth, hint.width());
                    //这里要看两行还是三行，确定是否要换列
                    if (2 == rowCount) {
                        //两行模式，换列
                        //换列，x自动递增到下个坐标，列数增加，行数归零，最大列宽归零
                        x += (columMaxWidth + spacing);
                        row = 0;
                        columMaxWidth = 0;
                        ++column;
                    } else {
                        //三行模式，继续增加行数
                        row = 2;
                        //x不变
                    }
                } else {
                    //第三行
                    item->rowIndex = 2;
                    item->columnIndex = column;
                    item->itemWillSetGeometry = QRect(x, ySmallRow2, hint.width(), smallHeight);
                    //和上个进行比较得到最长宽度
                    columMaxWidth = qMax(columMaxWidth, hint.width());
                    //换列，x自动递增到下个坐标，列数增加，行数归零，最大列宽归零
                    x += (columMaxWidth + spacing);
                    row = 0;
                    columMaxWidth = 0;
                    ++column;
                }
            }
                break;

            default:
                //不可能出现
                break;
        }
        //最后一个元素，更新列数
        if (i == (itemCount - 1)) { //最后一个元素，更新totalWidth
            if (item->columnIndex != column) {
                //说明最后一个元素处于最后位置，触发了换列，此时真实列数需要减1，直接等于column索引
                m_columnCount = column;
                //由于最后一个元素触发了换列，x值是新一列的位置，直接作为totalWidth
                totalWidth = x + mag.right();
            } else {
                //说明最后一个元素处于非最后位置，没有触发下一个换列，此时真实列数等于column索引+1
                m_columnCount = column + 1;
                //由于最后一个元素未触发换列，需要计算totalWidth
                totalWidth = x + columMaxWidth + spacing + mag.right();
            }
        }
    }
    //在有optionButton情况下，的2行模式，需要调整totalWidth
    if (pannel->isTwoRow()) {
        if (pannel->isHaveOptionAction()) {
            totalWidth += pannel->optionActionButtonSize().width();
        }
    }
    //在设置完所有窗口后，再设置扩展属性的窗口
    if (totalWidth < setrect.width()) {
        //说明可以设置扩展属性的窗口
        recalcExpandGeomArray(setrect);
    }
    this->m_sizeHint = QSize(totalWidth, height);
}


void RibbonPannelLayout::recalcExpandGeomArray(const QRect &setrect) {
    //计算能扩展的尺寸
    int expandwidth = setrect.width() - this->m_sizeHint.width();

    if (expandwidth <= 0) {
        //没有必要设置
        return;
    }
    //列扩展信息
    struct _columnExpandInfo {
        int oldColumnWidth = 0;             ///< 原来的列宽
        int columnMaximumWidth = -1;        ///< 列的最大宽度
        int columnExpandedWidth = 0;        ///< 扩展后列的宽度
        QList<RibbonPannelItem *> expandItems;
    };
    //此变量用于记录可以水平扩展的列和控件，在布局结束后，如果还有空间，就把水平扩展的控件进行扩展
    QMap<int, _columnExpandInfo> columnExpandInfo;

    for (RibbonPannelItem *item:m_items) {
        if ((!item->isEmpty()) && item->expandingDirections() & Qt::Horizontal) {
            //只获取可见的
            QMap<int, _columnExpandInfo>::iterator i = columnExpandInfo.find(item->columnIndex);
            if (i == columnExpandInfo.end()) {
                i = columnExpandInfo.insert(item->columnIndex, _columnExpandInfo());
            }
            i.value().expandItems.append(item);
        }
    }
    if (columnExpandInfo.size() <= 0) {
        //没有需要扩展的就退出
        return;
    }
    //获取完可扩展的列和控件后，计算对应的列的尺寸
    //计算能扩展的尺寸
    int oneColCanexpandWidth = expandwidth / columnExpandInfo.size();

    for (auto i = columnExpandInfo.begin(); i != columnExpandInfo.end();) {
        int &oldColumnWidth = i.value().oldColumnWidth;
        int &columnMaximumWidth = i.value().columnMaximumWidth;
        columnWidthInfo(i.key(), oldColumnWidth, columnMaximumWidth);
        if ((oldColumnWidth <= 0) || (oldColumnWidth > columnMaximumWidth)) {
            //如果小于0说明没有这个列，这种属于异常，删除继续
            //oldColumnWidth > columnMaximumWidth也是异常
            i = columnExpandInfo.erase(i);
            continue;
        }
        //开始调整
        int colwidth = oneColCanexpandWidth + oldColumnWidth;//先扩展了
        if (colwidth >= columnMaximumWidth) {
            //过最大宽度要求
            i.value().columnExpandedWidth = columnMaximumWidth;
        } else {
            i.value().columnExpandedWidth = colwidth;
        }
        ++i;
    }
    //从新调整尺寸
    //由于会涉及其他列的变更，因此需要所有都遍历一下
    for (auto i = columnExpandInfo.begin(); i != columnExpandInfo.end(); ++i) {
        int moveXLen = i.value().columnExpandedWidth - i.value().oldColumnWidth;
        for (RibbonPannelItem *item:m_items) {
            if (item->isEmpty() || (item->columnIndex < i.key())) {
                //之前的列不用管
                continue;
            }
            if (item->columnIndex == i.key()) {
                //此列的扩展
                if (i.value().expandItems.contains(item)) {
                    //此列需要扩展的item才扩展尺寸
                    item->itemWillSetGeometry.setWidth(i.value().columnExpandedWidth);
                } else {
                    //此列不扩展的模块保持原来的尺寸
                    continue;
                }
            } else {
                //后面的移动
                item->itemWillSetGeometry.moveLeft(item->itemWillSetGeometry.x() + moveXLen);
            }
        }
    }
}

/**
 * @brief 根据列数，计算窗口的宽度，以及最大宽度
 * @param colindex
 * @param width 如果传入没有这个列，返回-1
 * @param maximum 如果传入没有这个列，返回-1
 */
void RibbonPannelLayout::columnWidthInfo(int colindex, int &width, int &maximum) const {
    width = -1;
    maximum = -1;
    for (RibbonPannelItem *item:m_items) {
        if (!item->isEmpty() && (item->columnIndex == colindex)) {
            width = qMax(width, item->itemWillSetGeometry.width());
            maximum = qMax(maximum, item->widget()->maximumWidth());
        }
    }
}


void RibbonPannelLayout::setGeometry(const QRect &rect) {
    m_dirty = false;
    updateGeomArray(rect);
    QLayout::setGeometry(rect);
    layoutActions();
}

/**************************************************/
/** ------------ RibbonPannel ------------**/
/**************************************************/


RibbonPannel::RibbonPannel(QWidget *parent) : QWidget(parent), m_d(new RibbonPannelPrivate(this)) {
    setPannelLayoutMode(ThreeRowMode);
}


RibbonPannel::~RibbonPannel() {
    delete m_d;
}


/**
 * @brief 设置action的行行为，行属性决定了ribbon pannel的显示方式
 * @param action 需要设置的action，此action必须已经被pannel添加过
 * @param rp 行为
 */
void RibbonPannel::setActionRowProportion(QAction *action, RibbonPannelItem::RowProportion rp) {
    RibbonPannelLayout *lay = m_d->m_layout;

    if (lay) {
        RibbonPannelItem *it = lay->pannelItem(action);
        if (it) {
            it->rowProportion = rp;
            lay->invalidate();
        }
    }
}

/**
 * @brief 添加action
 * @param action action
 * @param rp 指定action的行占比
 * @return 返回对应的RibbonToolButton，如果是窗口，返回的toolbutton为nullptr
 */
RibbonToolButton *RibbonPannel::addAction(QAction *action, RibbonPannelItem::RowProportion rp) {
    m_d->m_lastRp = rp;
    addAction(action);
    return (m_d->lastAddActionButton());
}


/**
 * @brief 添加大图标
 *
 * @param action
 * @ 如果想获取actiom对应的RibbonToolButton,可以使用@ref actionToRibbonToolButton 函数
 */
RibbonToolButton *RibbonPannel::addLargeAction(QAction *action) {
    return (addAction(action, RibbonPannelItem::Large));
}


/**
 * @brief 在三栏模式下，强制加为2栏action
 * @note 在两行模式下，Medium和Small等价
 * 主要应用在ThreeRowMode下
 * @param action
 * @ 如果想获取actiom对应的RibbonToolButton,可以使用@ref actionToRibbonToolButton 函数
 */
RibbonToolButton *RibbonPannel::addMediumAction(QAction *action) {
    return (addAction(action, RibbonPannelItem::Medium));
}


/**
 * @brief 添加小图标
 * @param action
 * @ 如果想获取actiom对应的RibbonToolButton,可以使用@ref actionToRibbonToolButton 函数
 */
RibbonToolButton *RibbonPannel::addSmallAction(QAction *action) {
    return (addAction(action, RibbonPannelItem::Small));
}


/**
 * @brief 生成并添加一个action
 *
 * 如果不对此action做操作，RibbonPannel将管理此action
 *
 * @note action的父对象将设置为RibbonPannel，RibbonPannel在删除时将会删除子对象，会把这个action也删除，
 * 如果不想此action也删除，需要对action重新设置父对象
 *
 * @param text action的文字
 * @param icon action的图标
 * @param popMode 按钮的样式
 * @param rp action在pannel中的占位情况，默认是大图标
 * @return 返回添加的action
 */
QAction *RibbonPannel::addAction(const QString &text, const QIcon &icon, QToolButton::ToolButtonPopupMode popMode,
                                 RibbonPannelItem::RowProportion rp) {
    QAction *action = new QAction(icon, text, this);

    m_d->m_lastRp = rp;
    addAction(action);
    RibbonToolButton *btn = m_d->lastAddActionButton();

    if (btn) {
        btn->setPopupMode(popMode);
    }
    return (action);
}


/**
 * @brief 添加一个普通菜单
 * @param menu
 * @param rp
 * @param popMode,菜单弹出模式，默认InstantPopup模式
 * @return
 */
RibbonToolButton *
RibbonPannel::addMenu(QMenu *menu, RibbonPannelItem::RowProportion rp, QToolButton::ToolButtonPopupMode popMode) {
    QAction *action = menu->menuAction();

    addAction(action, rp);
    RibbonToolButton *btn = m_d->lastAddActionButton();

    btn->setPopupMode(popMode);
    return (btn);
}


/**
 * @brief 添加一个ActionMenu
 * @param action
 * @param menu
 * @param rp
 * @return
 */
RibbonToolButton *RibbonPannel::addActionMenu(QAction *action, QMenu *menu, RibbonPannelItem::RowProportion rp) {
    addAction(action, rp);
    RibbonToolButton *btn = m_d->lastAddActionButton();

    btn->setMenu(menu);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    return (btn);
}


/**
 * @brief 添加action menu,action menu是一个特殊的menu,即可点击触发action，也可弹出菜单
 * @param action 点击触发的action，在pannel中，图标以此action的图标为准
 * @param menu 需要弹出的menu
 * @return 返回
 */
RibbonToolButton *RibbonPannel::addLargeActionMenu(QAction *action, QMenu *menu) {
    return (addActionMenu(action, menu, RibbonPannelItem::Large));
}


RibbonToolButton *RibbonPannel::addLargeMenu(QMenu *menu, QToolButton::ToolButtonPopupMode popMode) {
    return (addMenu(menu, RibbonPannelItem::Large, popMode));
}


RibbonToolButton *RibbonPannel::addSmallMenu(QMenu *menu, QToolButton::ToolButtonPopupMode popMode) {
    return (addMenu(menu, RibbonPannelItem::Small, popMode));
}


/**
 * @brief 添加窗口
 *
 * @param w
 * @param rp
 * @return 返回action(QWidgetAction)
 * @note RibbonPannel并不会管理此窗口内存，在delete RibbonPannel时，此窗口如果父对象不是RibbonPannel将不会被删除
 */
QAction *RibbonPannel::addWidget(QWidget *w, RibbonPannelItem::RowProportion rp) {
    QWidgetAction *action = new QWidgetAction(this);

    action->setDefaultWidget(w);
    w->setAttribute(Qt::WA_Hover);
    m_d->m_lastRp = rp;
    addAction(action);
    return (action);
}


/**
 * @brief 添加窗口,占用ribbon的一行
 * @param w
 * @return 返回action(QWidgetAction)
 */
QAction *RibbonPannel::addSmallWidget(QWidget *w) {
    return (addWidget(w, RibbonPannelItem::Small));
}


/**
 * @brief 添加窗口，占用所有行
 * @param w
 * @return 返回action(QWidgetAction)
 */
QAction *RibbonPannel::addLargeWidget(QWidget *w) {
    return (addWidget(w, RibbonPannelItem::Large));
}


/**
 * @brief RibbonPannel::addGallery
 * @return
 * @note RibbonPannel将拥有RibbonGallery的管理权
 */
RibbonGallery *RibbonPannel::addGallery() {
    RibbonGallery *gallery = RibbonSubElementDelegate->createRibbonGallery(this);

    addWidget(gallery, RibbonPannelItem::Large);

    setExpanding();
    return (gallery);
}


/**
 * @brief 添加分割线
 * @param top 上边距 @default 6
 * @param bottom 下边距 @default 6
 */
QAction *RibbonPannel::addSeparator(int top, int bottom) {
    QAction *action = new QAction(this);

    action->setSeparator(true);
    m_d->m_lastRp = RibbonPannelItem::Large;
    addAction(action);
    QWidget *w = m_d->m_layout->lastWidget();
    RibbonSeparatorWidget *sep = qobject_cast<RibbonSeparatorWidget *>(w);

    if (sep) {
        sep->setTopBottomMargins(top, bottom);
    }
    return (action);
}


/**
 * @brief 从pannel中把action对应的button提取出来，如果action没有对应的button，就返回nullptr
 * @param action
 * @return 如果action没有对应的button，就返回nullptr
 */
RibbonToolButton *RibbonPannel::actionToRibbonToolButton(QAction *action) {
    RibbonPannelLayout *lay = qobject_cast<RibbonPannelLayout *>(layout());

    if (lay) {
        int index = lay->indexOf(action);
        if (index == -1) {
            return (nullptr);
        }
        QLayoutItem *item = lay->takeAt(index);
        RibbonToolButton *btn = qobject_cast<RibbonToolButton *>(item ? item->widget() : nullptr);
        return (btn);
    }
    return (nullptr);
}


/**
 * @brief 获取pannel下面的所有toolbutton
 * @return
 */
QList<RibbonToolButton *> RibbonPannel::ribbonToolButtons() const {
    const QObjectList &objs = children();
    QList<RibbonToolButton *> res;

    for (QObject *o : objs) {
        RibbonToolButton *b = qobject_cast<RibbonToolButton *>(o);
        if (b) {
            res.append(b);
        }
    }
    return (res);
}


/**
 * @brief 设置PannelLayoutMode
 * @param mode
 */
void RibbonPannel::setPannelLayoutMode(RibbonPannel::PannelLayoutMode mode) {
    if (m_d->m_pannelLayoutMode == mode) {
        return;
    }
    m_d->m_pannelLayoutMode = mode;
    resetLayout(mode);
    resetLargeToolButtonStyle();
}


RibbonPannel::PannelLayoutMode RibbonPannel::pannelLayoutMode() const {
    return (m_d->m_pannelLayoutMode);
}


/**
 * @brief 添加操作action，如果要去除，传入nullptr指针即可，RibbonPannel不会对QAction的所有权进行管理
 * @param action
 * @note 要去除OptionAction直接传入nullptr即可
 * @note RibbonPannel不对QAction的destroy进行关联，如果外部对action进行delete，需要先传入nullptr给addOptionAction
 */
void RibbonPannel::addOptionAction(QAction *action) {
    if (nullptr == action) {
        if (m_d->m_optionActionButton) {
            delete m_d->m_optionActionButton;
            m_d->m_optionActionButton = nullptr;
        }
        return;
    }
    if (nullptr == m_d->m_optionActionButton) {
        m_d->m_optionActionButton = RibbonSubElementDelegate->createRibbonPannelOptionButton(this);
    }
    m_d->m_optionActionButton->setFixedSize(optionActionButtonSize());
    m_d->m_optionActionButton->setIconSize(optionActionButtonSize() - QSize(-2, -2));
    m_d->m_optionActionButton->connectAction(action);
    updateGeometry(); //通知layout进行重新布局
    repaint();
}


/**
 * @brief 判断是否存在OptionAction
 * @return 存在返回true
 */
bool RibbonPannel::isHaveOptionAction() const {
    return (m_d->m_optionActionButton != nullptr);
}


QSize RibbonPannel::maxHightIconSize(const QSize &size, int h) {
    if (size.height() < h) {
        return (size * ((float) h / size.height()));
    }
    return (size);
}


void RibbonPannel::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    //! 1. 绘制标题
    if (ThreeRowMode == pannelLayoutMode()) {
        const int th = titleHeight();
        QFont f = font();
        f.setPixelSize(th * 0.6);
        p.setFont(f);
        if (m_d->m_optionActionButton) {
            p.drawText(1, height() - th, width() - m_d->m_optionActionButton->width() - 4, th, Qt::AlignCenter,
                       windowTitle());
        } else {
            p.drawText(1, height() - th, width(), th, Qt::AlignCenter, windowTitle());
        }
    }

    QWidget::paintEvent(event);
}


QSize RibbonPannel::sizeHint() const {
    QSize laySize = layout()->sizeHint();
    int maxWidth = laySize.width() + 2;

    if (ThreeRowMode == pannelLayoutMode()) {
        //三行模式
        QFontMetrics fm = fontMetrics();
        QSize titleSize = fm.size(Qt::TextShowMnemonic, windowTitle());
        if (m_d->m_optionActionButton) {
            //optionActionButton的宽度需要预留
            titleSize.setWidth(titleSize.width() + m_d->m_optionActionButton->width() + 4);
        }
        maxWidth = qMax(maxWidth, titleSize.width());
    }
    return (QSize(maxWidth, laySize.height()));
}


QSize RibbonPannel::minimumSizeHint() const {
    return (layout()->minimumSize());
}


/**
 * @brief 把pannel设置为扩展模式，此时会撑大水平区域
 * @param isExpanding
 */
void RibbonPannel::setExpanding(bool isExpanding) {
    setSizePolicy(isExpanding ? QSizePolicy::Expanding : QSizePolicy::Preferred, QSizePolicy::Fixed);
}


/**
 * @brief 判断此pannel是否为（水平）扩展模式
 * @return 是扩展模式返回true
 */
bool RibbonPannel::isExpanding() const {
    QSizePolicy sp = sizePolicy();

    return (sp.horizontalPolicy() == QSizePolicy::Expanding);
}


/**
 * @brief 标题栏高度，仅在三行模式下生效
 * @return
 */
int RibbonPannel::titleHeight() const {
    return (isTwoRow() ? 0 : 21);
}


/**
 * @brief 返回optionActionButton的尺寸
 * @return
 */
QSize RibbonPannel::optionActionButtonSize() const {
    return (isTwoRow() ? QSize(12, 12) : QSize(16, 16));
}


/**
 * @brief action对应的布局index，此操作一般用于移动，其他意义不大
 * @param act
 * @return 没有查到返回-1
 */
int RibbonPannel::actionIndex(QAction *act) const {
    return (m_d->m_layout->indexOf(act));
}


/**
 * @brief 移动action
 * @param from 要移动action的位置，通过@ref actionIndex 获取
 * @param to 要移动的位置
 */
void RibbonPannel::moveAction(int from, int to) {
    m_d->m_layout->move(from, to);
    updateGeometry(); //通知layout进行重新布局
}


/**
 * @brief 判断是否可以自定义
 * @return
 */
bool RibbonPannel::isCanCustomize() const {
    return (m_d->m_isCanCustomize);
}


/**
 * @brief 设置是否可以自定义
 * @param b
 */
void RibbonPannel::setCanCustomize(bool b) {
    m_d->m_isCanCustomize = b;
}


void RibbonPannel::resetLayout(PannelLayoutMode newmode) {
    Q_UNUSED(newmode);
    layout()->setSpacing(TwoRowMode == newmode ? 4 : 2);
    updateGeometry(); //通知layout进行重新布局
}


/**
 * @brief 重置大按钮的类型
 */
void RibbonPannel::resetLargeToolButtonStyle() {
    QList<RibbonToolButton *> btns = ribbonToolButtons();

    for (RibbonToolButton *b : btns) {
        if ((nullptr == b) || (RibbonToolButton::LargeButton != b->buttonType())) {
            continue;
        }
        if (ThreeRowMode == pannelLayoutMode()) {
            if (RibbonToolButton::Normal != b->largeButtonType()) {
                b->setLargeButtonType(RibbonToolButton::Normal);
            }
        } else {
            if (RibbonToolButton::Lite != b->largeButtonType()) {
                b->setLargeButtonType(RibbonToolButton::Lite);
            }
        }
    }
}


void RibbonPannel::resizeEvent(QResizeEvent *event) {
    //! 1.移动操作按钮到角落
    if (m_d->m_optionActionButton) {
        if (ThreeRowMode == pannelLayoutMode()) {
            m_d->m_optionActionButton->move(width() - m_d->m_optionActionButton->width() - 2, height() - titleHeight()
                                                                                              + (titleHeight() -
                                                                                                 m_d->m_optionActionButton->height()) /
                                                                                                2);
        } else {
            m_d->m_optionActionButton->move(width() - m_d->m_optionActionButton->width(),
                                            height() - m_d->m_optionActionButton->height());
        }
    }
    //! 2.resize后，重新设置分割线的高度
    //! 由于分割线在布局中，只要分割线足够高就可以，不需要重新设置
    return (QWidget::resizeEvent(event));
}


/**
 * @brief 处理action的事件
 *
 * 这里处理了ActionAdded，ActionChanged，ActionRemoved三个事件
 *
 * ActionAdded时向布局请求，添加action，布局中同时触发了@ref RibbonPannelLayout::createItem 函数
 * 此函数用于生成窗口，例如QRibbonToolButton
 *
 * ActionChanged时会让布局重新计算尺寸，并向category请求重新布局，有可能category的所有要重新调整尺寸
 *
 * ActionRemoved会移除布局管理的QLayoutItem，并进行内存清理，这时窗口也会隐藏，同时销毁
 *
 * @param e
 * @note 所有action事件都会向category请求重新布局
 *
 */
void RibbonPannel::actionEvent(QActionEvent *e) {
    QAction *action = e->action();
    QWidgetAction *widgetAction = qobject_cast<QWidgetAction *>(action);

    switch (e->type()) {
        case QEvent::ActionAdded: {
            RibbonPannelLayout *lay = m_d->m_layout;
            if (nullptr != widgetAction) {
                if (widgetAction->parent() != this) {
                    widgetAction->setParent(this);
                }
            }
            int index = layout()->count();
            if (e->before()) {
                //说明是插入
                index = lay->indexOf(action);
                if (-1 == index) {
                    index = layout()->count(); //找不到的时候就插入到最后
                }
            }
            lay->insertAction(index, action, m_d->m_lastRp);
            m_d->m_lastRp = RibbonPannelItem::None; //插入完后重置为None
            //由于pannel的尺寸发生变化，需要让category也调整
            QApplication::postEvent(parentWidget(), new QEvent(QEvent::LayoutRequest));
        }
            break;

        case QEvent::ActionChanged: {
            //让布局重新绘制
            layout()->invalidate();
            //由于pannel的尺寸发生变化，需要让category也调整
            QApplication::postEvent(parentWidget(), new QEvent(QEvent::LayoutRequest));
        }
            break;

        case QEvent::ActionRemoved: {
            RibbonPannelLayout *lay = m_d->m_layout;
            action->disconnect(this);
            int index = lay->indexOf(action);
            if (index != -1) {
                QLayoutItem *item = lay->takeAt(index);
                delete item;
            }
            //由于pannel的尺寸发生变化，需要让category也调整
            QApplication::postEvent(parentWidget(), new QEvent(QEvent::LayoutRequest));
        }
            break;

        default:
            break;
    }
}


/**
 * @brief 获取布局item
 * @return
 */
const QList<RibbonPannelItem *> &RibbonPannel::ribbonPannelItem() const {
    return (m_d->m_layout->m_items);
}


