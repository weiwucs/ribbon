//
// Created by weiwu on 21-5-1.
//

#include <QVariant>
#include <QDebug>
#include <QMetaType>

#include "RibbonBar.h"
#include "RibbonControlButton.h"
#include "RibbonElementManager.h"
#include "RibbonStackedWidget.h"

class _ContextCategoryManagerData {
public:
    RibbonContextCategory *contextCategory;
    QList<int> tabPageIndex;

    bool operator==(const RibbonContextCategory *contextPage) {
        return (this->contextCategory == contextPage);
    }
};

class _RibbonTabData {
public:
    RibbonCategory *category;
    int index;

    _RibbonTabData() : category(nullptr), index(-1) {
    }
};

Q_DECLARE_METATYPE(_RibbonTabData)

class RibbonBarPrivate {
public:
    RibbonBar *MainClass;
    QAbstractButton *applitionButton;
    RibbonTabBar *ribbonTabBar;
    RibbonStackedWidget *stackedContainerWidget;
    QList<_ContextCategoryManagerData> currentShowingContextCategory;
    QList<RibbonContextCategory *> mContextCategoryList;          ///< 存放所有的上下文标签
    QList<_RibbonTabData> mHidedCategory;
    int iconRightBorderPosition;                                    ///< 标题栏x值得最小值，在有图标和快捷启动按钮，此值都需要变化
    RibbonControlButton *minimumCaterogyButton;                   ///< 隐藏面板按钮
    RibbonButtonGroupWidget *tabBarRightSizeButtonGroupWidget;    ///< 在tab bar旁边的button group widget                                    ///< tabbar底部的线条颜色
    RibbonQuickAccessBar *quickAccessBar;                         ///< 快速响应栏
    RibbonBar::RibbonStyle ribbonStyle;                           ///< ribbon的风格
    RibbonBar::RibbonStyle lastShowStyle;                         ///< ribbon的风格
    RibbonBar::RibbonState currentRibbonMode;                     ///< 记录当前模式
    QSize windowButtonSize;                                         ///< 由RibbonMainWindow告诉的windowbutton的尺寸
    QList<QColor> mContextCategoryColorList;                        ///< contextCategory的色系
    int mContextCategoryColorListIndex;                             ///< 记录contextCategory色系索引
    RibbonBarPrivate(RibbonBar *par)
            : applitionButton(nullptr), ribbonTabBar(nullptr), stackedContainerWidget(nullptr),
              iconRightBorderPosition(1), minimumCaterogyButton(nullptr), tabBarRightSizeButtonGroupWidget(nullptr),
              ribbonStyle(RibbonBar::OfficeStyle), lastShowStyle(RibbonBar::OfficeStyle),
              currentRibbonMode(RibbonBar::NormalRibbonMode),
              windowButtonSize(100, RibbonSubElementStyleOpt.titleBarHight), mContextCategoryColorListIndex(-1) {
        MainClass = par;
        mContextCategoryColorList << QColor(201, 89, 156) // 玫红
                                  << QColor(242, 203, 29) // 黄
                                  << QColor(255, 157, 0)  // 橙
                                  << QColor(14, 81, 167)  // 蓝
                                  << QColor(228, 0, 69)   // 红
                                  << QColor(67, 148, 0)   // 绿
                ;
    }


    void init() {
        applitionButton = RibbonSubElementDelegate->createRibbonApplicationButton(MainClass);
        MainClass->connect(applitionButton, &QAbstractButton::clicked, MainClass, &RibbonBar::applitionButtonClicked);
        ribbonTabBar = RibbonSubElementDelegate->createRibbonTabBar(MainClass);
        ribbonTabBar->setObjectName("objRibbonTabBar");
        ribbonTabBar->setDrawBase(false);
        MainClass->connect(ribbonTabBar, &QTabBar::currentChanged, MainClass, &RibbonBar::onCurrentRibbonTabChanged);
        MainClass->connect(ribbonTabBar, &QTabBar::tabBarClicked, MainClass, &RibbonBar::onCurrentRibbonTabClicked);
        MainClass->connect(ribbonTabBar, &QTabBar::tabBarDoubleClicked, MainClass,
                           &RibbonBar::onCurrentRibbonTabDoubleClicked);
        MainClass->connect(ribbonTabBar, &QTabBar::tabMoved, MainClass, &RibbonBar::onTabMoved);
        //
        stackedContainerWidget = RibbonSubElementDelegate->createRibbonStackedWidget(MainClass);
        ribbonTabBar->setObjectName("objStackedContainerWidget");
        stackedContainerWidget->connect(stackedContainerWidget, &RibbonStackedWidget::hidWindow, MainClass,
                                        &RibbonBar::onStackWidgetHided);
        stackedContainerWidget->installEventFilter(MainClass);
        setNormalMode();
        //
        quickAccessBar = RibbonSubElementDelegate->createQuickAccessBar(MainClass);
        quickAccessBar->setObjectName("objRibbonQuickAccessBar");
    }


    void setApplitionButton(QAbstractButton *btn) {
        if (applitionButton) {
            delete applitionButton;
        }
        if (btn) {
            if (btn->parent() != MainClass) {
                btn->setParent(MainClass);
            }
            btn->move(0, RibbonSubElementStyleOpt.titleBarHight);
            MainClass->connect(applitionButton, &QAbstractButton::clicked, MainClass,
                               &RibbonBar::applitionButtonClicked);
        }
        applitionButton = btn;
    }


    bool isContainContextCategoryInList(RibbonContextCategory *contextCategory) {
        for (int i = 0; i < currentShowingContextCategory.size(); ++i) {
            if (currentShowingContextCategory[i] == contextCategory) {
                return (true);
            }
        }
        return (false);
    }


    void setHideMode() {
        currentRibbonMode = RibbonBar::MinimumRibbonMode;
        this->stackedContainerWidget->setPopupMode();
        this->stackedContainerWidget->setFocusPolicy(Qt::NoFocus);
        this->stackedContainerWidget->clearFocus();
        this->ribbonTabBar->setFocus();
        this->stackedContainerWidget->hide();
        MainClass->setFixedHeight(ribbonTabBar->geometry().bottom());
    }


    void setNormalMode() {
        currentRibbonMode = RibbonBar::NormalRibbonMode;
        this->stackedContainerWidget->setNormalMode();
        this->stackedContainerWidget->setFocus();
        this->stackedContainerWidget->show();
    }


    QColor getContextCategoryColor() {
        if (mContextCategoryColorList.isEmpty()) {
            mContextCategoryColorListIndex = -1;
            return (QColor());
        }
        ++mContextCategoryColorListIndex;
        if ((mContextCategoryColorListIndex >= mContextCategoryColorList.size()) ||
            (mContextCategoryColorListIndex < 0)) {
            mContextCategoryColorListIndex = 0;
        }
        return (mContextCategoryColorList.at(mContextCategoryColorListIndex));
    }
};


/**
 * @brief 判断样式是否为两行
 * @param s
 * @return 两行返回true，返回false代表当前是3行
 */
bool RibbonBar::isTwoRowStyle(RibbonBar::RibbonStyle s) {
    return ((s & 0xFF00) > 0);
}


/**
 * @brief 判断是否是office样式
 * @param s
 * @return Office样式返回true，否则就是wps样式
 */
bool RibbonBar::isOfficeStyle(RibbonBar::RibbonStyle s) {
    return ((s & 0xFF) == 0);
}


/**
 * @brief RibbonBar构造函数
 * @param parent
 */
RibbonBar::RibbonBar(QWidget *parent) : QMenuBar(parent), m_d(new RibbonBarPrivate(this)) {
    m_d->init();
    connect(parent, &QWidget::windowTitleChanged, this, &RibbonBar::onWindowTitleChanged);
    connect(parent, &QWidget::windowIconChanged, this, &RibbonBar::onWindowIconChanged);
    setRibbonStyle(OfficeStyle);
}


RibbonBar::~RibbonBar() {
    delete m_d;
}


/**
 * @brief 返回applicationButton
 * @return 默认的applicationButton是@ref RibbonApplicationButton 生成，通过@ref setApplicationButton 可设置为其他button
 */
QAbstractButton *RibbonBar::applicationButton() {
    return (m_d->applitionButton);
}


/**
 * @brief 设置applicationButton,如果想隐藏，可以传入nullptr
 *
 * 默认会有一个RibbonApplicationButton，如果想取消，可传入nullptr，或者自定义的button也可以传入
 *
 * @note applicationButton的所有权归RibbonBar所有，不要在外部对applicationButton进行delete操作
 * @param btn applicationButton指针，可以传入@ref RibbonApplicationButton，
 * 已经对RibbonApplicationButton进行了样式设置
 */
void RibbonBar::setApplicationButton(QAbstractButton *btn) {
    m_d->setApplitionButton(btn);
    if (btn) {
        if (btn->objectName().isEmpty()) {
            btn->setObjectName("RibbonApplitionButton");
        }
        btn->setVisible(true);
        //btn->setGeometry(applitionButtonGeometry());
    }
    //无论设置为什么都触发resize
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
}


/**
 * @brief 返回tabbar
 * @return RibbonTabBar指针
 * @ RibbonTabBar
 */
RibbonTabBar *RibbonBar::ribbonTabBar() {
    return (m_d->ribbonTabBar);
}


/**
 * @brief 添加一个标签
 * 如果需要删除，直接delete即可，RibbonBar会对其进行处理
 * @param title 标签名字，默认情况下RibbonCategory的object name也被设置为title
 * @return 返回一个窗口容器，在Category里可以添加其他控件
 * @ RibbonCategory
 */
RibbonCategory *RibbonBar::addCategoryPage(const QString &title) {
    RibbonCategory *catagory = RibbonSubElementDelegate->createRibbonCategory(this);

    //catagory->setFixedHeight(categoryHeight());
    catagory->setObjectName(title);
    catagory->setWindowTitle(title);
    int index = m_d->ribbonTabBar->addTab(title);

    catagory->setRibbonPannelLayoutMode(isTwoRowStyle() ? RibbonPannel::TwoRowMode : RibbonPannel::ThreeRowMode);

    _RibbonTabData tabdata;

    tabdata.category = catagory;
    tabdata.index = index;
    m_d->ribbonTabBar->setTabData(index, QVariant::fromValue(tabdata));

    m_d->stackedContainerWidget->insertWidget(index, catagory);
    connect(catagory, &QWidget::windowTitleChanged, this, &RibbonBar::onCategoryWindowTitleChanged);
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
    //销毁时移除tab
    return (catagory);
}


/**
 * @brief 添加一个category，category的位置在index，如果当前category数量少于index，将插入到最后
 * @param title category的标题
 * @param index category的位置
 * @return
 */
RibbonCategory *RibbonBar::insertCategoryPage(const QString &title, int index) {
    RibbonCategory *catagory = RibbonSubElementDelegate->createRibbonCategory(this);

    catagory->setObjectName(title);
    catagory->setWindowTitle(title);
    catagory->setRibbonPannelLayoutMode(isTwoRowStyle() ? RibbonPannel::TwoRowMode : RibbonPannel::ThreeRowMode);
    int i = m_d->ribbonTabBar->insertTab(index, title);

    _RibbonTabData tabdata;

    tabdata.category = catagory;
    tabdata.index = i;
    m_d->ribbonTabBar->setTabData(i, QVariant::fromValue(tabdata));
    m_d->stackedContainerWidget->insertWidget(index, catagory);

    connect(catagory, &QWidget::windowTitleChanged, this, &RibbonBar::onCategoryWindowTitleChanged);
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
    //销毁时移除tab
    return (catagory);
}


/**
 * @brief 通过名字查找Category
 * @param title Category的名字，既标签的标题
 * @return 如果没有找到，将返回nullptr，如果有重名，将返回第一个查询到的名字，因此，尽量避免重名标签
 * @note 由于翻译等原因，可能title会变化，因此如果想通过固定内容查找category，应该使用 @ref categoryByObjectName
 * @see categoryByObjectName
 */
RibbonCategory *RibbonBar::categoryByName(const QString &title) const {
    int c = m_d->stackedContainerWidget->count();

    for (int i = 0; i < c; ++i) {
        RibbonCategory *w = qobject_cast<RibbonCategory *>(m_d->stackedContainerWidget->widget(i));
        if (w) {
            if (w->windowTitle() == title) {
                return (w);
            }
        }
    }
    return (nullptr);
}


/**
 * @brief 通过ObjectName查找Category
 * @param objname
 * @return 如果没有找到，将返回nullptr，如果有同样的ObjectName，将返回第一个查询到的名字，因此，尽量避免ObjectName重名
 * @see categoryByName
 */
RibbonCategory *RibbonBar::categoryByObjectName(const QString &objname) const {
    int c = m_d->stackedContainerWidget->count();

    for (int i = 0; i < c; ++i) {
        RibbonCategory *w = qobject_cast<RibbonCategory *>(m_d->stackedContainerWidget->widget(i));
        if (w) {
            if (w->objectName() == objname) {
                return (w);
            }
        }
    }
    return (nullptr);
}


/**
 * @brief 通过索引找到category，如果超过索引范围，会返回nullptr
 * @param index 索引
 * @return 如果超过索引范围，会返回nullptr
 * @note 如果此时有上下文标签，上下文的标签也会返回
 * @note 通过索引查找的category必须是visible状态的category，如果通过@ref hideCategory 隐藏的标签，通过索引是找不到的
 * @note 通过@ref categoryByObjectName 可以找到所有加入过的标签，
 * 如果想得到ribbonbar管理的所有标签，可以通过函数@ref categoryPages 得到
 * @see categoryIndex categoryByObjectName categoryByName
 */
RibbonCategory *RibbonBar::categoryByIndex(int index) const {
    QVariant var = m_d->ribbonTabBar->tabData(index);

    if (var.isValid()) {
        _RibbonTabData p = var.value<_RibbonTabData>();
        return (p.category);
    }
    return (nullptr);
}


/**
 * @brief 隐藏category,并不会删除或者取走，只是隐藏
 * @param category
 */
void RibbonBar::hideCategory(RibbonCategory *category) {
    int tabcount = m_d->ribbonTabBar->count();

    for (int i = 0; i < tabcount; ++i) {
        QVariant var = m_d->ribbonTabBar->tabData(i);
        if (var.isValid()) {
            _RibbonTabData p = var.value<_RibbonTabData>();
            if (p.category == category) {
                m_d->mHidedCategory.append(p);
                m_d->ribbonTabBar->removeTab(i);//仅仅把tab移除
            }
        }
    }
}


/**
 * @brief 显示被隐藏的category
 * @param category
 */
void RibbonBar::showCategory(RibbonCategory *category) {
    for (auto i = m_d->mHidedCategory.begin(); i != m_d->mHidedCategory.end(); ++i) {
        if (i->category == category) {
            //说明要显示
            int index = m_d->ribbonTabBar->insertTab(i->index, i->category->windowTitle());
            i->index = index;
            m_d->ribbonTabBar->setTabData(index, QVariant::fromValue(*i));
            m_d->mHidedCategory.erase(i);//移除
            return;
        }
    }
}


/**
 * @brief 判断这个category是否在显示状态，也就是tabbar有这个category
 * @param category
 * @return
 */
bool RibbonBar::isCategoryVisible(RibbonCategory *category) const {
    int tabindex = categoryIndex(category);

    return (tabindex >= 0);
}


/**
 * @brief 获取category的索引
 * @param c
 * @return 如果找不到，返回-1
 */
int RibbonBar::categoryIndex(RibbonCategory *c) const {
    int tabcount = m_d->ribbonTabBar->count();

    for (int i = 0; i < tabcount; ++i) {
        QVariant var = m_d->ribbonTabBar->tabData(i);
        if (var.isValid()) {
            _RibbonTabData p = var.value<_RibbonTabData>();
            if (p.category == c) {
                return (i);
            }
        }
    }
    return (-1);
}


/**
 * @brief 移动一个Category从from index到to index
 * @param from
 * @param to
 */
void RibbonBar::moveCategory(int from, int to) {
    m_d->ribbonTabBar->moveTab(from, to);
    //这时要刷新所有tabdata的index信息
    int tabcount = m_d->ribbonTabBar->count();

    for (int i = 0; i < tabcount; ++i) {
        QVariant var = m_d->ribbonTabBar->tabData(i);
        if (var.isValid()) {
            _RibbonTabData p = var.value<_RibbonTabData>();
            p.index = i;
            m_d->ribbonTabBar->setTabData(i, QVariant::fromValue(p));
        }
    }
    //这里会触发tabMoved信号，在tabMoved信号中调整stacked里窗口的位置
}


/**
 * @brief 获取当前显示的所有的RibbonCategory，包含未显示的RibbonContextCategory的RibbonCategory也一并返回
 *
 * @return
 */
QList<RibbonCategory *> RibbonBar::categoryPages(bool getAll) const {
    int c = m_d->stackedContainerWidget->count();
    QList<RibbonCategory *> res;

    for (int i = 0; i < c; ++i) {
        RibbonCategory *w = qobject_cast<RibbonCategory *>(m_d->stackedContainerWidget->widget(i));
        if (w) {
            if (!getAll && w->isContextCategory()) {
                //不是getall且是上下文时跳过
                continue;
            }
            res.append(w);
        }
    }
    return (res);
}


/**
 * @brief 移除RibbonCategory
 *
 * RibbonBar不会delete RibbonCategory*，但这个RibbonCategory会脱离RibbonBar的管理
 * 表现在tabbar会移除，面板会移除，使用此函数后可以对RibbonCategory进行delete
 * @param category
 */
void RibbonBar::removeCategory(RibbonCategory *category) {
    int index = tabIndex(category);

    if (index >= 0) {
        m_d->ribbonTabBar->removeTab(index);
    }
    m_d->stackedContainerWidget->removeWidget(category);
    //同时验证这个category是否是contexcategory里的
    for (RibbonContextCategory *c : m_d->mContextCategoryList) {
        c->takeCategory(category);
        updateContextCategoryManagerData();
    }
    //移除完后需要重绘
    repaint();
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
}


/**
 * @brief 添加上下文标签
 *
 * 上下文标签是特殊时候触发的标签，需要用户手动触发
 *
 * 调用@ref RibbonContextCategory::addCategoryPage 可在上下文标签中添加RibbonCategory，
 * 在上下文标签添加的RibbonCategory，只有在上下文标签显示的时候才会显示
 * @param title 上下文标签的标题，在Office模式下会显示，在wps模式下不显示。默认情况下RibbonContextCategory的object name也被设置为title
 * @param color 上下文标签的颜色，如果指定为空QColor(),将会使用RibbonBar的默认色系
 * @param id 上下文标签的id，以便进行查找
 * @return 返回上下文标签指针
 * @note RibbonBar拥有RibbonContextCategory的管理权，用户避免在外部直接delete,如果要删除，调用@ref destroyContextCategory 函数
 */
RibbonContextCategory *
RibbonBar::addContextCategory(const QString &title, const QColor &color, const QVariant &id) {
    RibbonContextCategory *context = RibbonSubElementDelegate->createRibbonContextCategory(this);

    context->setObjectName(title);
    context->setContextTitle(title);
    context->setId(id);
    context->setContextColor(color.isValid() ? color : m_d->getContextCategoryColor());
    connect(context, &RibbonContextCategory::categoryPageAdded, this, &RibbonBar::onContextsCategoryPageAdded);
    //remove并没有绑定，主要是remove后在stacked里也不会显示，remove且delete的话，stacked里也会删除
    if (currentRibbonStyle() == WpsLiteStyle) {
        resizeInWpsLiteStyle();
    }
    m_d->mContextCategoryList.append(context);
    return (context);
}


/**
 * @brief 显示上下文标签
 * @param context 上下文标签指针
 */
void RibbonBar::showContextCategory(RibbonContextCategory *context) {
    if (isContextCategoryVisible(context)) {
        return;
    }
    _ContextCategoryManagerData contextCategoryData;

    contextCategoryData.contextCategory = context;
    for (int i = 0; i < context->categoryCount(); ++i) {
        RibbonCategory *category = context->categoryPage(i);
        category->setRibbonPannelLayoutMode(
                isTwoRowStyle() ? RibbonPannel::TwoRowMode : RibbonPannel::ThreeRowMode);
        //切换模式后会改变高度，上下文标签显示时要保证显示出来
        //category->setFixedHeight(categoryHeight());
        int index = m_d->ribbonTabBar->addTab(category->windowTitle());
        contextCategoryData.tabPageIndex.append(index);

        _RibbonTabData tabdata;
        tabdata.category = category;
        tabdata.index = index;
        m_d->ribbonTabBar->setTabData(index, QVariant::fromValue(tabdata));
    }
    m_d->currentShowingContextCategory.append(contextCategoryData);
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
    repaint();
}


/**
 * @brief 隐藏上下文标签
 * @param context 上下文标签指针
 */
void RibbonBar::hideContextCategory(RibbonContextCategory *context) {
    for (int i = 0; i < m_d->currentShowingContextCategory.size(); ++i) {
        if (m_d->currentShowingContextCategory[i].contextCategory == context) {
            const QList<int> &indexs = m_d->currentShowingContextCategory[i].tabPageIndex;
            for (int j = indexs.size() - 1; j >= 0; --j) {
                m_d->ribbonTabBar->removeTab(indexs[j]);
            }
            m_d->currentShowingContextCategory.removeAt(i);
        }
    }
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
    repaint();
}


/**
 * @brief 判断上下文是否在显示状态
 * @param context
 * @return 在显示状态返回true
 * @ setContextCategoryVisible
 */
bool RibbonBar::isContextCategoryVisible(RibbonContextCategory *context) {
    return (m_d->isContainContextCategoryInList(context));
}


/**
 * @brief 设置上下文标签的显示状态
 *
 * 上下文标签的当前显示状态可通过 @ref isContextCategoryVisible 进行判断
 * @param context 上下文标签
 * @param visible 显示状态，true为显示
 */
void RibbonBar::setContextCategoryVisible(RibbonContextCategory *context, bool visible) {
    if (visible) {
        showContextCategory(context);
    } else {
        hideContextCategory(context);
    }
}


/**
 * @brief 获取所有的上下文标签
 * @return 返回上下文标签列表
 */
QList<RibbonContextCategory *> RibbonBar::contextCategoryList() const {
    return (m_d->mContextCategoryList);
}


/**
 * @brief 销毁上下文标签，上下文标签的RibbonCategory也会随之销毁
 * @param context 需要销毁的上下文标签指针
 */
void RibbonBar::destroyContextCategory(RibbonContextCategory *context) {
    //! 1、如果上下文标签显示中，先隐藏
    if (isContextCategoryVisible(context)) {
        hideContextCategory(context);
    }
    //! 2、删除上下文标签的相关内容
    m_d->mContextCategoryList.removeAll(context);
    //!
    QList<RibbonCategory *> res = context->categoryList();

    for (RibbonCategory *c : res) {
        c->hide();
        c->deleteLater();
    }
    context->deleteLater();
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
}


/**
 * @brief 设置为最小/正常模式
 *
 * 隐藏模式下，只会显示tabbar，不会显示内容，默认状态是显示模式
 *
 * 默认下双击tabbar会切换隐藏显示模式，如果想禁用此功能，可以重载 @ref onCurrentRibbonTabDoubleClicked
 * 函数，不对函数进行任何处理即可
 *
 * @param isMinimum 参数为true时，切换为Minimum模式
 * @see 此函数会改变@ref RibbonState 状态，通过@ref currentRibbonState 函数可以查看当前状态
 */
void RibbonBar::setMinimumMode(bool isMinimum) {
    if (isMinimum) {
        m_d->setHideMode();
    } else {
        m_d->setNormalMode();
    }
    QResizeEvent resizeEvent(size(), size());
    QApplication::sendEvent(this, &resizeEvent);
}


///
/// \brief 当前ribbon是否是隐藏模式
/// \return
///
bool RibbonBar::isMinimumMode() const {
    return (m_d->stackedContainerWidget->isPopupMode());
}


///
/// \brief 设置显示隐藏ribbon按钮
///
void RibbonBar::showMinimumModeButton(bool isShow) {
    if (isShow) {
        activeTabBarRightButtonGroup();
        if (nullptr == m_d->minimumCaterogyButton) {
            m_d->minimumCaterogyButton = RibbonSubElementDelegate->createHidePannelButton(this);
            QAction *action = new QAction(m_d->minimumCaterogyButton);
            action->setCheckable(true);
            action->setChecked(isMinimumMode());
            action->setIcon(QIcon(":/icon/icon/ve.png"));
            connect(action, &QAction::triggered, this, [this](bool on) {
                this->setMinimumMode(on);
            });
            m_d->minimumCaterogyButton->setDefaultAction(action);
            m_d->tabBarRightSizeButtonGroupWidget->addWidget(m_d->minimumCaterogyButton);
            update();
        }
    } else {
        if (nullptr != m_d->minimumCaterogyButton) {
            m_d->minimumCaterogyButton->hide();
            m_d->minimumCaterogyButton->deleteLater();
            m_d->minimumCaterogyButton = nullptr;
        }
    }
    QResizeEvent resizeEvent(size(), size());

    QApplication::sendEvent(this, &resizeEvent);
}


///
/// \brief 是否显示隐藏ribbon按钮
/// \return
///
bool RibbonBar::haveShowMinimumModeButton() const {
    return (nullptr != m_d->minimumCaterogyButton);
}


int RibbonBar::tabBarHeight() const {
    return (RibbonSubElementStyleOpt.tabBarHight);
}


int RibbonBar::titleBarHeight() const {
    return (RibbonSubElementStyleOpt.titleBarHight);
}


void RibbonBar::onWindowTitleChanged(const QString &title) {
    Q_UNUSED(title);
    update();
}


void RibbonBar::onWindowIconChanged(const QIcon &icon) {
    if (!icon.isNull()) {
        int iconMinSize = RibbonSubElementStyleOpt.titleBarHight - 6;
        QSize s = icon.actualSize(QSize(iconMinSize, iconMinSize));
        m_d->iconRightBorderPosition = RibbonSubElementStyleOpt.widgetBord.left() + s.width();
    }
    update();
}


void RibbonBar::onCategoryWindowTitleChanged(const QString &title) {
    RibbonCategory *w = qobject_cast<RibbonCategory *>(sender());

    if (nullptr == w) {
        return;
    }
    for (int i = 0; i < m_d->ribbonTabBar->count(); ++i) {
        //鉴于tab不会很多，不考虑效率问题
        QVariant var = m_d->ribbonTabBar->tabData(i);
        if (var.isValid()) {
            _RibbonTabData p = var.value<_RibbonTabData>();
            if (w == p.category) {
                m_d->ribbonTabBar->setTabText(i, title);
            }
        }
    }
}


///
/// \brief ribbon的显示界面隐藏
///
void RibbonBar::onStackWidgetHided() {
    //m_d->ribbonTabBar->repaint();
}


/**
 * @brief 标签切换触发槽函数
 * @param index
 */
void RibbonBar::onCurrentRibbonTabChanged(int index) {
    QVariant var = m_d->ribbonTabBar->tabData(index);
    RibbonCategory *category = nullptr;

    if (var.isValid()) {
        _RibbonTabData p = var.value<_RibbonTabData>();
        category = p.category;
    }
    if (category) {
        if (m_d->stackedContainerWidget->currentWidget() != category) {
            m_d->stackedContainerWidget->setCurrentWidget(category);
        }
        if (isMinimumMode()) {
            m_d->ribbonTabBar->clearFocus();
            if (!m_d->stackedContainerWidget->isVisible()) {
                if (m_d->stackedContainerWidget->isPopupMode()) {
                    //在stackedContainerWidget弹出前，先给tabbar一个QHoverEvent,让tabbar知道鼠标已经移开
                    QHoverEvent ehl(QEvent::HoverLeave, m_d->ribbonTabBar->mapToGlobal(QCursor::pos()),
                                    m_d->ribbonTabBar->mapToGlobal(QCursor::pos())
                    );
                    QApplication::sendEvent(m_d->ribbonTabBar, &ehl);
                    resizeStackedContainerWidget();
                    m_d->stackedContainerWidget->setFocus();
                    m_d->stackedContainerWidget->exec();
                    //在最小模式下，每次显示完stackedContainerWidget后把tab的
                    //的index设置为-1，这样每次点击都会触发onCurrentRibbonTabChanged
                }
            } else {
            }
        }
    }
    emit currentRibbonTabChanged(index);
}


/**
 * @brief ribbon tab bar单击
 *
 * 此实现必须在eventfilter中传递stackedwidget的QEvent::MouseButtonDblClick事件到tabbar中，否则会导致双击变两次单击
 *
 * 单击事件仅用于响应点击同一个tab时
 * @param index
 */
void RibbonBar::onCurrentRibbonTabClicked(int index) {
    if (index != m_d->ribbonTabBar->currentIndex()) {
        //点击的标签不一致通过changed槽去处理
        return;
    }
    if (this->isMinimumMode()) {
        if (!this->m_d->stackedContainerWidget->isVisible()) {
            if (this->m_d->stackedContainerWidget->isPopupMode()) {
                //在stackedContainerWidget弹出前，先给tabbar一个QHoverEvent,让tabbar知道鼠标已经移开
                QHoverEvent ehl(QEvent::HoverLeave, m_d->ribbonTabBar->mapToGlobal(QCursor::pos()),
                                m_d->ribbonTabBar->mapToGlobal(QCursor::pos())
                );
                QApplication::sendEvent(m_d->ribbonTabBar, &ehl);
                //弹出前都调整一下位置，避免移动后位置异常
                resizeStackedContainerWidget();
                this->m_d->stackedContainerWidget->setFocus();
                this->m_d->stackedContainerWidget->exec();
            }
        }
    }
}


/**
 * @brief ribbon tab bar双击
 *
 * 默认情况下双击会切换最小和正常模式
 * @param index
 */
void RibbonBar::onCurrentRibbonTabDoubleClicked(int index) {
    Q_UNUSED(index);
    setMinimumMode(!isMinimumMode());
}


void RibbonBar::onContextsCategoryPageAdded(RibbonCategory *category) {
    Q_ASSERT_X(category != nullptr, "onContextsCategoryPageAdded", "add nullptr page");
    m_d->stackedContainerWidget->addWidget(category);//这里stackedWidget用append，其他地方都应该使用insert
}


/**
 * @brief 标签移动的信号
 * @param from
 * @param to
 */
void RibbonBar::onTabMoved(int from, int to) {
    //调整stacked widget的顺序，调整顺序是为了调用categoryPages函数返回的QList<RibbonCategory *>顺序和tabbar一致
    m_d->stackedContainerWidget->moveWidget(from, to);
}


/**
 * @brief 根据RibbonCategory*指针查找tabbar的index
 *
 * @param c RibbonCategory对应的QObject指针
 * @return 如果没有找到，返回-1
 * @note 此函数不会调用RibbonCategory*的任何方法，因此可以在RibbonCategory的destroyed槽中调用
 */
int RibbonBar::tabIndex(RibbonCategory *obj) {
    const int size = m_d->ribbonTabBar->count();

    for (int i = 0; i < size; ++i) {
        QVariant v = m_d->ribbonTabBar->tabData(i);
        if (v.isValid()) {
            _RibbonTabData innervalue = v.value<_RibbonTabData>();
            if (innervalue.category == obj) {
                return (i);
            }
        }
    }
    //如果找不到就从stackedwidget中找

    return (-1);
}


void RibbonBar::updateRibbonElementGeometry() {
    //根据样式调整RibbonCategory的布局形式
    QList<RibbonCategory *> categorys = categoryPages();

    for (RibbonCategory *c : categorys) {
        c->setRibbonPannelLayoutMode(RibbonBar::isTwoRowStyle(currentRibbonStyle()) ? RibbonPannel::TwoRowMode
                                                                                    : RibbonPannel::ThreeRowMode);
    }

    //根据样式调整bar的高度
    if (NormalRibbonMode == currentRibbonState()) {
        setFixedHeight(mainBarHeight());
    }
//    //最小模式时，bar的高度在resize之后调整
//    else if(MinimumRibbonMode == currentRibbonState()){
//        //处于最小模式下时，bar的高度为tabbar的bottom
//        setFixedHeight(m_d->ribbonTabBar->geometry().bottom());
//    }
}


RibbonButtonGroupWidget *RibbonBar::activeTabBarRightButtonGroup() {
    if (nullptr == m_d->tabBarRightSizeButtonGroupWidget) {
        m_d->tabBarRightSizeButtonGroupWidget = RibbonSubElementDelegate->craeteButtonGroupWidget(this);
        m_d->tabBarRightSizeButtonGroupWidget->setFrameShape(QFrame::NoFrame);
        m_d->tabBarRightSizeButtonGroupWidget->show();
    }
    if (!m_d->tabBarRightSizeButtonGroupWidget->isVisible()) {
        m_d->tabBarRightSizeButtonGroupWidget->setVisible(true);
    }
    return (m_d->tabBarRightSizeButtonGroupWidget);
}


RibbonQuickAccessBar *RibbonBar::quickAccessBar() {
    return (m_d->quickAccessBar);
}


/**
 * @brief 设置ribbonbar的风格，此函数会重新设置所有元素，包括button的布局方式，
 * 尤其是从三行变到两行的过程，重设的内容较多
 * @note 此函数会自动触发ResizeEvent，不需要手动调用
 * @param v 样式，见@ref RibbonBar::RibbonStyle
 */
void RibbonBar::setRibbonStyle(RibbonBar::RibbonStyle v) {
    m_d->ribbonStyle = v;
    m_d->lastShowStyle = v;
    updateRibbonElementGeometry();
    QSize oldSize = size();
    QSize newSize(oldSize.width(), mainBarHeight());
    QResizeEvent es(newSize, oldSize);

    QApplication::sendEvent(this, &es);
    if (MinimumRibbonMode == currentRibbonState()) {
        //处于最小模式下时，bar的高度为tabbar的bottom,这个调整必须在resize event之后
        setFixedHeight(m_d->ribbonTabBar->geometry().bottom());
    }
}


/**
 * @brief 返回当前ribbon的风格
 * @return 返回当前ribbon的风格
 */
RibbonBar::RibbonStyle RibbonBar::currentRibbonStyle() const {
    return (m_d->ribbonStyle);
}


RibbonBar::RibbonState RibbonBar::currentRibbonState() const {
    return (m_d->currentRibbonMode);
}


///
/// \brief 切换到对应标签
/// \param index 标签索引
///
void RibbonBar::setCurrentIndex(int index) {
    m_d->ribbonTabBar->setCurrentIndex(index);
    //onCurrentRibbonTabChanged(index);
}


///
/// \brief 确保标签显示出来，tab并切换到对应页
/// \param category 标签指针
///
void RibbonBar::raiseCategory(RibbonCategory *category) {
    int index = m_d->stackedContainerWidget->indexOf(category);

    if (index >= 0) {
        setCurrentIndex(index);
    }
}


/**
 * @brief 判断当前的样式是否为两行
 * @return
 */
bool RibbonBar::isTwoRowStyle() const {
    return (RibbonBar::isTwoRowStyle(m_d->ribbonStyle));
}


/**
 * @brief 判断当前的样式是否为office样式
 * @return
 */
bool RibbonBar::isOfficeStyle() const {
    return (RibbonBar::isOfficeStyle(currentRibbonStyle()));
}


/**
 * @brief 告诉ribbonbar，window button的尺寸,此值由RibbonMainWindow传入，
 * 告诉最大，最小，关闭按钮的大小，在显示标题栏的时候好计算尺寸
 * @param size
 */
void RibbonBar::setWindowButtonSize(const QSize &size) {
    m_d->windowButtonSize = size;
}


bool RibbonBar::eventFilter(QObject *obj, QEvent *e) {
    if (obj) {
        //调整多文档时在窗口模式下的按钮更新
        if ((obj == cornerWidget(Qt::TopLeftCorner)) || (obj == cornerWidget(Qt::TopRightCorner))) {
            if ((QEvent::UpdateLater == e->type()) ||
                (QEvent::MouseButtonRelease == e->type()) ||
                (QEvent::WindowActivate == e->type())) {
                QApplication::postEvent(this, new QResizeEvent(size(), size()));
            }
        } else if (obj == m_d->stackedContainerWidget) {
            //在stack 是popup模式时，点击的是stackedContainerWidget区域外的时候，如果是在ribbonTabBar上点击
            //那么忽略掉这次点击，把点击下发到ribbonTabBar,这样可以避免stackedContainerWidget在点击ribbonTabBar后
            //隐藏又显示，出现闪烁
            if ((QEvent::MouseButtonPress == e->type()) || (QEvent::MouseButtonDblClick == e->type())) {
                if (m_d->stackedContainerWidget->isPopupMode()) {
                    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
                    if (!m_d->stackedContainerWidget->rect().contains(mouseEvent->pos())) {
                        QWidget *clickedWidget = QApplication::widgetAt(mouseEvent->globalPos());
                        if (clickedWidget == m_d->ribbonTabBar) {
                            const QPoint targetPoint = clickedWidget->mapFromGlobal(mouseEvent->globalPos());
                            QMouseEvent *evPress = new QMouseEvent(mouseEvent->type(), targetPoint,
                                                                   mouseEvent->globalPos(), mouseEvent->button(),
                                                                   mouseEvent->buttons(), mouseEvent->modifiers());
                            QApplication::postEvent(clickedWidget, evPress);
                            return (true);
                        }
                    }
                }
            }
        }
//        if (WindowButtonGroup *g = qobject_cast<WindowButtonGroup *>(obj)) {
//            if (e->type() == QEvent::Resize) {
//                m_d->windowButtonSize = g->size();
//            }
//        }
    }
    return (QMenuBar::eventFilter(obj, e));
}


/**
 * @brief 根据情况重置tabbar的宽度，主要针对wps模式
 */
int RibbonBar::calcMinTabBarWidth() const {
    //20200831
    //tabBarWidth的宽度原来为endX - x;，现需要根据实际进行调整
    //为了把tabbar没有tab的部分不占用，这里的宽度需要根据tab的size来进行设置，让tabbar的长度刚刚好，这样能让出
    //mainwindow的空间，接受鼠标事件，从而实现拖动等操作，否则tabbar占用整个顶栏，鼠标无法点击到mainwindow
    //计算tab所占用的宽度
    const QMargins &mg = m_d->ribbonTabBar->tabMargin();
    int mintabBarWidth = 0;

    for (int i = 0; i < m_d->ribbonTabBar->count(); ++i) {
        mintabBarWidth += m_d->ribbonTabBar->tabRect(i).width();
    }
    mintabBarWidth += (mg.left() + mg.right());
    return (mintabBarWidth);
}


/**
 * @brief mainBarHeight的计算高度
 *
 * 有可能RibbonBar::height和mainBarHeight不相等，这种情况发生在RibbonState::MinimumRibbonMode状态下
 * @return 高度
 */
int RibbonBar::mainBarHeight() const {
    switch (currentRibbonStyle()) {
        case OfficeStyle:
            return (RibbonSubElementStyleOpt.mainbarHeightOfficeStyleThreeRow);

        case WpsLiteStyle:
            return (RibbonSubElementStyleOpt.mainbarHeightWPSStyleThreeRow);

        case OfficeStyleTwoRow:
            //之前如果是3行，要改变所有的Category行数
            return (RibbonSubElementStyleOpt.mainbarHeightOfficeStyleTwoRow);

        case WpsLiteStyleTwoRow:
            return (RibbonSubElementStyleOpt.mainbarHeightWPSStyleTwoRow);

        default:
            break;
    }
    return (RibbonSubElementStyleOpt.mainbarHeightOfficeStyleThreeRow);
}


int RibbonBar::applitionButtonWidth() const {
    return (56);
}


void RibbonBar::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    if (isOfficeStyle()) {
        paintInNormalStyle();
    } else {
        paintInWpsLiteStyle();
    }
#ifdef _RIBBON_DEBUG_HELP_DRAW
    QPainter p(this);
    HELP_DRAW_RECT(p, m_d->quickAccessBar->geometry());
    HELP_DRAW_RECT(p, m_d->ribbonTabBar->geometry());
    HELP_DRAW_RECT(p, m_d->stackedContainerWidget->geometry());
#endif
}


void RibbonBar::paintInNormalStyle() {
    QPainter p(this);

    //!
    paintBackground(p);
    //! 显示上下文标签
    p.save();
    QList<_ContextCategoryManagerData> contextCategoryDataList = m_d->currentShowingContextCategory;
    bool isCurrentSelectContextCategoryPage = false;

    QPoint contextCategoryRegion(width(), -1);

    for (int i = 0; i < contextCategoryDataList.size(); ++i) {
        QRect contextTitleRect;
        QList<int> indexs = contextCategoryDataList[i].tabPageIndex;
        QColor clr = contextCategoryDataList[i].contextCategory->contextColor();
        if (!indexs.isEmpty()) {
            contextTitleRect = m_d->ribbonTabBar->tabRect(indexs.first());
            QRect endRect = m_d->ribbonTabBar->tabRect(indexs.last());
            contextTitleRect.setRight(endRect.right());
            contextTitleRect.translate(m_d->ribbonTabBar->x(), m_d->ribbonTabBar->y());
            contextTitleRect.setHeight(m_d->ribbonTabBar->height() - 1);//减1像素，避免tabbar基线覆盖
            contextTitleRect -= m_d->ribbonTabBar->tabMargin();
            //把区域顶部扩展到窗口顶部
            contextTitleRect.setTop(RibbonSubElementStyleOpt.widgetBord.top());
            //绘制
            paintContextCategoryTab(p, contextCategoryDataList[i].contextCategory->contextTitle(), contextTitleRect,
                                    clr);
            //更新上下文标签的范围，用于控制标题栏的显示
            if (contextTitleRect.left() < contextCategoryRegion.x()) {
                contextCategoryRegion.setX(contextTitleRect.left());
            }
            if (contextTitleRect.right() > contextCategoryRegion.y()) {
                contextCategoryRegion.setY(contextTitleRect.right());
            }
        }
        isCurrentSelectContextCategoryPage = indexs.contains(m_d->ribbonTabBar->currentIndex());
        if (isCurrentSelectContextCategoryPage) {
            QPen pen;
            pen.setColor(clr);
            pen.setWidth(1);
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawRect(m_d->stackedContainerWidget->geometry());
            isCurrentSelectContextCategoryPage = false;
        }
    }
    p.restore();
    //! 显示标题等
    QWidget *parWindow = parentWidget();

    if (parWindow) {
        QRect titleRegion;
        if (contextCategoryRegion.y() < 0) {
            titleRegion.setRect(m_d->quickAccessBar->geometry().right() + 1, RibbonSubElementStyleOpt.widgetBord.top(),
                                width() - m_d->iconRightBorderPosition - RibbonSubElementStyleOpt.widgetBord.right() -
                                m_d->windowButtonSize.width() - m_d->quickAccessBar->geometry().right() - 1,
                                RibbonSubElementStyleOpt.titleBarHight);
        } else {
            int leftwidth =
                    contextCategoryRegion.x() - m_d->quickAccessBar->geometry().right() - m_d->iconRightBorderPosition;
            int rightwidth = width() - contextCategoryRegion.y() - m_d->windowButtonSize.width();
//            if (width() - contextCategoryRegion.y() > contextCategoryRegion.x()-x) {
            if (rightwidth > leftwidth) {
                //说明右边的区域大一点，标题显示在右，显示在右边需要减去windowbutton宽度
                titleRegion.setRect(contextCategoryRegion.y(), RibbonSubElementStyleOpt.widgetBord.top(), rightwidth,
                                    RibbonSubElementStyleOpt.titleBarHight);
            } else {
                //说明左边的大一点
                titleRegion.setRect(m_d->iconRightBorderPosition + m_d->quickAccessBar->geometry().right(),
                                    RibbonSubElementStyleOpt.widgetBord.top(), leftwidth,
                                    RibbonSubElementStyleOpt.titleBarHight);
            }
        }
#ifdef _RIBBON_DEBUG_HELP_DRAW
        p.ve();
        p.setBrush(QColor(255, 0, 0, 120));
        p.drawRect(titleRegion);
        p.restore();
#endif
        paintWindowTitle(p, parWindow->windowTitle(), titleRegion);
        paintWindowIcon(p, parWindow->windowIcon());
    }
}


void RibbonBar::paintInWpsLiteStyle() {
    QPainter p(this);

    //!
    paintBackground(p);
    //! 显示上下文标签
    p.save();
    QList<_ContextCategoryManagerData> contextCategoryDataList = m_d->currentShowingContextCategory;
    bool isCurrentSelectContextCategoryPage = false;

    for (int i = 0; i < contextCategoryDataList.size(); ++i) {
        QRect contextTitleRect;
        QList<int> indexs = contextCategoryDataList[i].tabPageIndex;
        QColor clr = contextCategoryDataList[i].contextCategory->contextColor();
        if (!indexs.isEmpty()) {
            contextTitleRect = m_d->ribbonTabBar->tabRect(indexs.first());
            QRect endRect = m_d->ribbonTabBar->tabRect(indexs.last());
            contextTitleRect.setRight(endRect.right());
            contextTitleRect.translate(m_d->ribbonTabBar->x(), m_d->ribbonTabBar->y());
            contextTitleRect.setHeight(m_d->ribbonTabBar->height() - 1);
            contextTitleRect -= m_d->ribbonTabBar->tabMargin();
            //把区域顶部扩展到窗口顶部
            contextTitleRect.setTop(RibbonSubElementStyleOpt.widgetBord.top());
            //绘制
            paintContextCategoryTab(p, QString(), contextTitleRect, clr);
        }
        isCurrentSelectContextCategoryPage = indexs.contains(m_d->ribbonTabBar->currentIndex());
        if (isCurrentSelectContextCategoryPage) {
            QPen pen;
            pen.setColor(clr);
            pen.setWidth(1);
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            p.drawRect(m_d->stackedContainerWidget->geometry());
            isCurrentSelectContextCategoryPage = false;
        }
    }
    p.restore();
    //! 显示标题等


    QWidget *parWindow = parentWidget();

    if (parWindow) {
        int start = m_d->ribbonTabBar->x() + m_d->ribbonTabBar->width();
        int width = m_d->quickAccessBar->x() - start;
        if (width > 20) {
            QRect titleRegion(start, RibbonSubElementStyleOpt.widgetBord.top(), width,
                              RibbonSubElementStyleOpt.titleBarHight);
            paintWindowTitle(p, parWindow->windowTitle(), titleRegion);
            paintWindowIcon(p, parWindow->windowIcon());
        }
    }
}


void RibbonBar::resizeStackedContainerWidget() {
    if (m_d->stackedContainerWidget->isPopupMode()) {
        //弹出模式时，高度
        QPoint absPosition = mapToGlobal(
                QPoint(RibbonSubElementStyleOpt.widgetBord.left(), m_d->ribbonTabBar->geometry().bottom() + 1));
        m_d->stackedContainerWidget->setGeometry(absPosition.x(), absPosition.y(),
                                                 width() - RibbonSubElementStyleOpt.widgetBord.left() -
                                                 RibbonSubElementStyleOpt.widgetBord.right(),
                                                 mainBarHeight() - m_d->ribbonTabBar->geometry().bottom() -
                                                 RibbonSubElementStyleOpt.widgetBord.bottom() - 1);
    } else {
        m_d->stackedContainerWidget->setGeometry(RibbonSubElementStyleOpt.widgetBord.left(),
                                                 m_d->ribbonTabBar->geometry().bottom() + 1,
                                                 width() - RibbonSubElementStyleOpt.widgetBord.left() -
                                                 RibbonSubElementStyleOpt.widgetBord.right(),
                                                 mainBarHeight() - m_d->ribbonTabBar->geometry().bottom() -
                                                 RibbonSubElementStyleOpt.widgetBord.bottom() - 1);
    }
}


/**
 * @brief 刷新所有ContextCategoryManagerData，这个在单独一个Category删除时调用
 */
void RibbonBar::updateContextCategoryManagerData() {
    const int c = m_d->ribbonTabBar->count();

    for (_ContextCategoryManagerData &cd : m_d->currentShowingContextCategory) {
        cd.tabPageIndex.clear();
        for (int i = 0; i < cd.contextCategory->categoryCount(); ++i) {
            RibbonCategory *category = cd.contextCategory->categoryPage(i);
            for (int t = 0; t < c; ++t) {
                QVariant v = m_d->ribbonTabBar->tabData(t);
                if (v.isValid()) {
                    _RibbonTabData d = v.value<_RibbonTabData>();
                    if (d.category == category) {
                        cd.tabPageIndex.append(t);
                    }
                } else {
                    cd.tabPageIndex.append(-1);
                }
            }
        }
    }
}


/**
 * @brief 绘制上下文标签的背景
 * @param painter 绘图QPainter
 * @param title 上下文标签的title
 * @param contextRect 上下文标签的绘制区域
 * @param color 上下文标签赋予的颜色
 */
void
RibbonBar::paintContextCategoryTab(QPainter &painter, const QString &title, QRect contextRect, const QColor &color) {
    //绘制上下文标签
    //首先有5像素的实体粗线位于顶部
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRect(QRect(contextRect.x(), RibbonSubElementStyleOpt.widgetBord.top(), contextRect.width(), 5));

    //剩下把颜色变亮90%
    QColor gColor = color.light(190);

    //减去之前的5像素
    contextRect -= QMargins(0, 5, 0, 0);
    painter.fillRect(contextRect, gColor);

    //只有在office模式下才需要绘制标题
    if (isOfficeStyle()) {
        if (!title.isEmpty()) {
            contextRect.setBottom(m_d->ribbonTabBar->geometry().top());
            painter.setPen(color);
            painter.drawText(contextRect, Qt::AlignCenter, title);
        }
    }
    painter.restore();
}


void RibbonBar::resizeEvent(QResizeEvent *e) {
    Q_UNUSED(e);
    if (isOfficeStyle()) {
        resizeInOfficeStyle();
    } else {
        resizeInWpsLiteStyle();
    }
}


/**
 * @brief 重写moveevent是为了在移动时调整isPopupMode状态下的stackedContainerWidget位置
 * @param event
 */
void RibbonBar::moveEvent(QMoveEvent *event) {
    if (m_d->stackedContainerWidget) {
        if (m_d->stackedContainerWidget->isPopupMode()) {
            //弹出模式时，窗口发生了移动，同步调整StackedContainerWidget的位置
            qDebug() << "moveEvent";
            resizeStackedContainerWidget();
        }
    }
    QWidget::moveEvent(event);
}


void RibbonBar::resizeInOfficeStyle() {
    updateRibbonElementGeometry();
    int x = RibbonSubElementStyleOpt.widgetBord.left();
    int y = RibbonSubElementStyleOpt.widgetBord.top();

    //cornerWidget - TopLeftCorner
    const int titleH = titleBarHeight();
    const int validTitleBarHeight = titleH - RibbonSubElementStyleOpt.widgetBord.top();
    const int tabH = tabBarHeight();

    x += m_d->iconRightBorderPosition + 5;
    if (QWidget *connerL = cornerWidget(Qt::TopLeftCorner)) {
        if (connerL->isVisible()) {
            QSize connerSize = connerL->sizeHint();
            if (connerSize.height() < validTitleBarHeight) {
                int detal = (validTitleBarHeight - connerSize.height()) / 2;
                connerL->setGeometry(x, y + detal, connerSize.width(), connerSize.height());
            } else {
                connerL->setGeometry(x, y, connerSize.width(), validTitleBarHeight);
            }
            x = connerL->geometry().right() + 5;
        }
    }
    //quick access bar定位
    if (m_d->quickAccessBar) {
        if (m_d->quickAccessBar->isVisible()) {
            QSize quickAccessBarSize = m_d->quickAccessBar->sizeHint();
            m_d->quickAccessBar->setGeometry(x, y, quickAccessBarSize.width(), validTitleBarHeight);
        }
    }
    //第二行，开始布局applitionButton，tabbar，tabBarRightSizeButtonGroupWidget，TopRightCorner
    x = RibbonSubElementStyleOpt.widgetBord.left();
    y = titleH + RibbonSubElementStyleOpt.widgetBord.top();
    //applitionButton 定位
    if (m_d->applitionButton) {
        if (m_d->applitionButton->isVisible()) {
            m_d->applitionButton->setGeometry(x, y, applitionButtonWidth(), tabH);
            x = m_d->applitionButton->geometry().right();
        }
    }
    //top right是一定要配置的，对于多文档窗口，子窗口的缩放等按钮就是通过这个窗口实现，
    //由于这个窗口一定要在最右，因此先对这个窗口进行布局
    //cornerWidget - TopRightCorner
    //获取最右边的位置
    int endX = width() - RibbonSubElementStyleOpt.widgetBord.right();

    if (QWidget *connerW = cornerWidget(Qt::TopRightCorner)) {
        if (connerW->isVisible()) {
            QSize connerSize = connerW->sizeHint();
            endX -= connerSize.width();
            if (connerSize.height() < tabH) {
                int detal = (tabH - connerSize.height()) / 2;
                connerW->setGeometry(endX, y + detal, connerSize.width(), connerSize.height());
            } else {
                connerW->setGeometry(endX, y, connerSize.width(), tabH);
            }
        }
    }
    //applitionButton和TopRightCorner完成定位，才可以定位tab bar
    //tab bar 定位

    //tabBar 右边的附加按钮组，这里一般会附加一些类似登录等按钮组
    if (m_d->tabBarRightSizeButtonGroupWidget && m_d->tabBarRightSizeButtonGroupWidget->isVisible()) {
        QSize wSize = m_d->tabBarRightSizeButtonGroupWidget->sizeHint();
        endX -= wSize.width();
        m_d->tabBarRightSizeButtonGroupWidget->setGeometry(endX, y, wSize.width(), tabH);
    }
    //最后确定tabbar宽度
    int tabBarWidth = endX - x;

    m_d->ribbonTabBar->setGeometry(x, y, tabBarWidth, tabH);


    resizeStackedContainerWidget();
}


void RibbonBar::resizeInWpsLiteStyle() {
    updateRibbonElementGeometry();
    //m_d->lastShowStyle = WpsLiteStyle;
    int x = RibbonSubElementStyleOpt.widgetBord.left();
    int y = RibbonSubElementStyleOpt.widgetBord.top();
    const int titleH = titleBarHeight();
    const int validTitleBarHeight = titleH - RibbonSubElementStyleOpt.widgetBord.top();

    //applitionButton 定位
    if (m_d->applitionButton) {
        if (m_d->applitionButton->isVisible()) {
            m_d->applitionButton->setGeometry(x, y, applitionButtonWidth(), titleH);
            x = m_d->applitionButton->geometry().right() + 2;
        }
    }
    //applitionButton定位完后先布局右边内容
    //cornerWidget - TopRightCorner
    int endX = width() - RibbonSubElementStyleOpt.widgetBord.right() - m_d->windowButtonSize.width();

    if (QWidget *connerW = cornerWidget(Qt::TopRightCorner)) {
        if (connerW->isVisible()) {
            QSize connerSize = connerW->sizeHint();
            endX -= connerSize.width();
            if (connerSize.height() < validTitleBarHeight) {
                int detal = (validTitleBarHeight - connerSize.height()) / 2;
                connerW->setGeometry(endX, y + detal, connerSize.width(), connerSize.height());
            } else {
                connerW->setGeometry(endX, y, connerSize.width(), validTitleBarHeight);
            }
        }
    }
    //quick access bar定位
    if (m_d->quickAccessBar) {
        if (m_d->quickAccessBar->isVisible()) {
            QSize quickAccessBarSize = m_d->quickAccessBar->sizeHint();
            endX -= quickAccessBarSize.width();
            m_d->quickAccessBar->setGeometry(endX, y, quickAccessBarSize.width(), validTitleBarHeight);
        }
    }
    //cornerWidget - TopLeftCorner
    if (QWidget *connerL = cornerWidget(Qt::TopLeftCorner)) {
        if (connerL->isVisible()) {
            QSize connerSize = connerL->sizeHint();
            endX -= connerSize.width();
            if (connerSize.height() < validTitleBarHeight) {
                int detal = (validTitleBarHeight - connerSize.height()) / 2;
                connerL->setGeometry(endX, y + detal, connerSize.width(), connerSize.height());
            } else {
                connerL->setGeometry(endX, y, connerSize.width(), validTitleBarHeight);
            }
        }
    }
    //开始定位tabbar以及tabBarRightSizeButtonGroupWidget
    //tab bar 定位 wps模式下applitionButton的右边就是tab bar
    //tabBar 右边的附加按钮组
    if (m_d->tabBarRightSizeButtonGroupWidget && m_d->tabBarRightSizeButtonGroupWidget->isVisible()) {
        QSize wSize = m_d->tabBarRightSizeButtonGroupWidget->sizeHint();
        endX -= wSize.width();
        m_d->tabBarRightSizeButtonGroupWidget->setGeometry(endX, y, wSize.width(), validTitleBarHeight);
    }
    int tabBarWidth = endX - x;
    //20200831
    //tabBarWidth的宽度原来为endX - x;，现需要根据实际进行调整
    //为了把tabbar没有tab的部分不占用，这里的宽度需要根据tab的size来进行设置，让tabbar的长度刚刚好，这样能让出
    //mainwindow的空间，接受鼠标事件，从而实现拖动等操作，否则tabbar占用整个顶栏，鼠标无法点击到mainwindow
    //计算tab所占用的宽度
    int mintabBarWidth = calcMinTabBarWidth();

    if (mintabBarWidth < tabBarWidth) {
        tabBarWidth = mintabBarWidth;
    }
    //tab 的y值需要重新计算
    int tabH = tabBarHeight();

    if (tabH > validTitleBarHeight) {
        //这种直接把tabH设置为validTitleBarHeight
        tabH = validTitleBarHeight;
    }
    y = y + validTitleBarHeight - tabH;//如果tabH较小，则下以，让tab底部和title的底部对齐
    m_d->ribbonTabBar->setGeometry(x, y, tabBarWidth, tabH);

    //调整整个stackedContainer
    resizeStackedContainerWidget();
}


void RibbonBar::paintBackground(QPainter &painter) {
    painter.save();
    QPalette pl = palette();

    painter.setBrush(pl.background());
    painter.drawRect(rect());
    //在tabbar下绘制一条线
    const int lineY = m_d->ribbonTabBar->geometry().bottom();
    QPen pen(RibbonSubElementStyleOpt.tabBarBaseLineColor);

    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(QPoint(RibbonSubElementStyleOpt.widgetBord.left(), lineY),
                     QPoint(width() - RibbonSubElementStyleOpt.widgetBord.right() - 1, lineY));
    painter.restore();
}


///
/// \brief 绘制标题栏
/// \param painter
/// \param title 标题
/// \param contextCategoryRegion 当前显示的上下文标签的范围，上下文标签是可以遮挡标题栏的，因此需要知道上下文标签的范围
/// x表示左边界，y表示右边界
///
void RibbonBar::paintWindowTitle(QPainter &painter, const QString &title, const QRect &titleRegion) {
    painter.save();
    painter.setPen(RibbonSubElementStyleOpt.titleTextColor);
    painter.drawText(titleRegion, Qt::AlignCenter, title);
    painter.restore();
}


void RibbonBar::paintWindowIcon(QPainter &painter, const QIcon &icon) {
    painter.save();
    if (!icon.isNull()) {
        int iconMinSize = RibbonSubElementStyleOpt.titleBarHight - 6;
        icon.paint(&painter, RibbonSubElementStyleOpt.widgetBord.left() + 3,
                   RibbonSubElementStyleOpt.widgetBord.top() + 3, iconMinSize, iconMinSize);
        m_d->iconRightBorderPosition = RibbonSubElementStyleOpt.widgetBord.left() + iconMinSize;
    } else {
        m_d->iconRightBorderPosition = RibbonSubElementStyleOpt.widgetBord.left();
    }
    painter.restore();
}


