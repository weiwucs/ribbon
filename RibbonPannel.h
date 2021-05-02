//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONPANNEL_H
#define RIBBON_RIBBONPANNEL_H

#include <QWidget>
#include <QLayout>
#include "RibbonToolButton.h"
#include "RibbonGallery.h"

class RibbonPannelPrivate;

class RibbonPannelItem : public QWidgetItem{
public:
    enum RowProportion{
        None,Large,Medium,Small
    };
    RibbonPannelItem(QWidget *widget);
    bool isEmpty() const Q_DECL_OVERRIDE;

    short rowIndex;
    int columnIndex;
    QRect itemWillSetGeometry;
    QAction *action;
    bool customWidget;
    RibbonPannelItem::RowProportion rowProportion;
};

class RibbonPannel : public QWidget{

Q_OBJECT
    friend class RibbonCategory;
    friend class RibbonCategoryPrivate;
    friend class RibbonCustomizeWidgetPrivate;
    Q_PROPERTY(bool isCanCustomize READ isCanCustomize WRITE setCanCustomize)
    Q_PROPERTY(bool isExpanding READ isExpanding WRITE setExpanding)
public:
    RibbonPannel(QWidget *parent = 0);
    ~RibbonPannel();
    using QWidget::addAction;
    enum PannelLayoutMode {
        ThreeRowMode    ///< 三行布局模式，office就是三行布局模式，pannel能布置3行小toolbutton，默认模式
        , TwoRowMode    ///< 两行布局模式，wps的后续布局模式就是两行布局模式，pannel能布置2行小toolbutton
    };
    //设置action的行行为，行属性决定了ribbon pannel的显示方式
    void setActionRowProportion(QAction *action, RibbonPannelItem::RowProportion rp);

    //把action加入到pannel
    RibbonToolButton *addAction(QAction *action, RibbonPannelItem::RowProportion rp);

    //把action加入到pannel，并以大图标显示
    RibbonToolButton *addLargeAction(QAction *action);

    //把action加入到pannel，在三行模式下会以中图标显示
    RibbonToolButton *addMediumAction(QAction *action);

    //把action加入到pannel，并以小图标显示
    RibbonToolButton *addSmallAction(QAction *action);

    //生成并添加一个action
    QAction *addAction(const QString& text, const QIcon& icon
            , QToolButton::ToolButtonPopupMode popMode
            , RibbonPannelItem::RowProportion rp = RibbonPannelItem::Large);

    //添加menu
    RibbonToolButton *addMenu(QMenu *menu
            , RibbonPannelItem::RowProportion rp
            , QToolButton::ToolButtonPopupMode popMode = QToolButton::InstantPopup);

    //添加action menu
    RibbonToolButton *addActionMenu(QAction *action, QMenu *menu, RibbonPannelItem::RowProportion rp);

    //action menu,action menu是一个特殊的menu,即可点击触发action，也可弹出菜单
    RibbonToolButton *addLargeActionMenu(QAction *action, QMenu *menu);

    //添加普通大菜单
    RibbonToolButton *addLargeMenu(QMenu *menu, QToolButton::ToolButtonPopupMode popMode = QToolButton::InstantPopup);

    //添加普通小按钮菜单
    RibbonToolButton *addSmallMenu(QMenu *menu, QToolButton::ToolButtonPopupMode popMode = QToolButton::InstantPopup);

    //添加窗口
    QAction *addWidget(QWidget *w, RibbonPannelItem::RowProportion rp);

    //添加窗口,占用ribbon的一行
    QAction *addSmallWidget(QWidget *w);

    //添加窗口，占用所有行
    QAction *addLargeWidget(QWidget *w);

    //添加一个Gallery
    RibbonGallery *addGallery();

    //添加分割线
    QAction *addSeparator(int top = 6, int bottom = 6);

    //从pannel中把action对应的button提取出来，如果action没有对应的button，就返回nullptr
    RibbonToolButton *actionToRibbonToolButton(QAction *action);

    //添加操作action，如果要去除，传入nullptr指针即可，RibbonPannel不会对QAction的所有权进行管理
    void addOptionAction(QAction *action);

    //判断是否存在OptionAction
    bool isHaveOptionAction() const;

    //获取所有的buttons
    QList<RibbonToolButton *> ribbonToolButtons() const;

    //获取PannelLayoutMode
    PannelLayoutMode pannelLayoutMode() const;

    //判断是否为2行模式
    bool isTwoRow() const { return (TwoRowMode == pannelLayoutMode()); }

    virtual QSize sizeHint() const Q_DECL_OVERRIDE;
    virtual QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    //把pannel设置为扩展模式，此时会撑大水平区域
    void setExpanding(bool isExpanding = true);

    //是否是扩展模式
    bool isExpanding() const;

    //标题栏高度，仅在3行模式下生效
    virtual int titleHeight() const;

    //optionActionButton的尺寸
    virtual QSize optionActionButtonSize() const;

    //action对应的布局index，此操作一般用于移动moveAction，其他意义不大
    int actionIndex(QAction *act) const;

    //移动action
    void moveAction(int from, int to);

    //判断是否可以自定义
    bool isCanCustomize() const;
    void setCanCustomize(bool b);

signals:

    /**
     * @brief 等同于QToolBar::actionTriggered
     * @param action
     */
    void actionTriggered(QAction *action);

protected:
    //设置PannelLayoutMode，此函数设置为protect避免误调用
    void setPannelLayoutMode(PannelLayoutMode mode);
    void resetLayout(PannelLayoutMode newmode);
    void resetLargeToolButtonStyle();
    static QSize maxHightIconSize(const QSize& size, int h);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual void actionEvent(QActionEvent *e) Q_DECL_OVERRIDE;

    //获取布局对应的item
    const QList<RibbonPannelItem *>& ribbonPannelItem() const;

private:
    RibbonPannelPrivate *m_d;
};

class RibbonPannelLayout : public QLayout{
Q_OBJECT
    friend class RibbonPannel;
public:

    RibbonPannelLayout(QWidget *p = 0);
    ~RibbonPannelLayout();
    virtual int indexOf(QAction *action) const;

    //RibbonPannelLayout additem 无效
    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;

    //RibbonPannel主要通过此函数来添加action
    void insertAction(int index, QAction *act, RibbonPannelItem::RowProportion rp = RibbonPannelItem::None);

    //
    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;
    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;
    int count() const Q_DECL_OVERRIDE;

    bool isEmpty() const Q_DECL_OVERRIDE;
    void invalidate() Q_DECL_OVERRIDE;
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    void setGeometry(const QRect& rect) Q_DECL_OVERRIDE;
    QSize minimumSize() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    //通过action获取RibbonPannelItem
    RibbonPannelItem *pannelItem(QAction *action) const;

    //获取最后一个添加的item
    RibbonPannelItem *lastItem() const;

    //获取最后生成的窗口
    QWidget *lastWidget() const;

    //根据pannel的默认参数得到的pannel高度
    int defaultPannelHeight() const;

    //移动两个item
    void move(int from, int to);

    //判断是否需要重新布局
    bool isDirty() const;

protected:
    //布局action
    void layoutActions();

    //把action转换为item，对于纯Action，此函数会创建RibbonToolButton,
    //rp用于告诉Layout生成什么样的窗口，详细见RibbonPannelItem::RowProportion
    RibbonPannelItem *createItem(QAction *action, RibbonPannelItem::RowProportion rp = RibbonPannelItem::None);
    void updateGeomArray(const QRect& setrect);

    //重新计算扩展条码，此函数必须在updateGeomArray函数之后调用
    void recalcExpandGeomArray(const QRect& setrect);

private:
    //返回所有列的区域
    //QMap<int,QRect> columnsGeometry() const;
    //根据列数，计算窗口的宽度，以及最大宽度
    void columnWidthInfo(int colindex, int& width, int& maximum) const;

private:
    QList<RibbonPannelItem *> m_items;
    int m_columnCount;      ///< 记录有多少列
    bool m_expandFlag;      ///< 标记是否是会扩展的
    QSize m_sizeHint;       ///< sizeHint返回的尺寸
    bool m_dirty;           ///< 用于标记是否需要刷新元素，参考QToolBarLayout源码
    int m_largeHeight;      ///< 记录大图标的高度
};

#endif //RIBBON_RIBBONPANNEL_H
