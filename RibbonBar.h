//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONBAR_H
#define RIBBON_RIBBONBAR_H


#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAbstractButton>
#include "RibbonCategory.h"
#include "RibbonContextCategory.h"
#include "RibbonButtonGroupWidget.h"
#include "RibbonQuickAccessBar.h"
#include "RibbonTabBar.h"

class RibbonBarPrivate;

class RibbonBar : public QMenuBar {
    Q_OBJECT
    friend class RibbonBarPrivate;
public:

    /**
     * @brief 定义ribbon的风格,第一字节代表样式，第二字节代表是否是2行
     */
    enum RibbonStyle {
        OfficeStyle = 0x0000        ///< 类似office 的ribbon风格
        ,
        WpsLiteStyle = 0x0001        ///< 类似wps的紧凑风格
        ,
        OfficeStyleTwoRow = 0x0100        ///< 类似office 的ribbon风格 2行工具栏 三行布局模式，office就是三行布局模式，pannel能布置3行小toolbutton，默认模式
        ,
        WpsLiteStyleTwoRow = 0x0101        ///< 类似wps的紧凑风格  2行工具栏
    };
    Q_FLAGS(RibbonStyle)

    /**
     * @brief 定义当前ribbon 的状态
     */
    enum RibbonState {
        MinimumRibbonMode       ///< 缩小模式
        , NormalRibbonMode      ///< 正常模式
    };

    //判断RibbonStyle是否为2行模式
    static bool isTwoRowStyle(RibbonStyle s);

    //判断是否是office样式
    static bool isOfficeStyle(RibbonStyle s);

    //构造函数
    explicit RibbonBar(QWidget *parent = nullptr);

    ~RibbonBar();

    //获取applitionButton
    QAbstractButton *applicationButton();

    //设置applitionButton
    void setApplicationButton(QAbstractButton *btn);

    //获取tabbar
    RibbonTabBar *ribbonTabBar();

    //添加一个标签
    RibbonCategory *addCategoryPage(const QString &title);

    //添加一个category，category的位置在index，如果当前category数量少于index，将插入到最后
    RibbonCategory *insertCategoryPage(const QString &title, int index);

    //通过名字查找Category
    RibbonCategory *categoryByName(const QString &title) const;

    //通过ObjectName查找Category
    RibbonCategory *categoryByObjectName(const QString &objname) const;

    //通过索引找到category，如果超过索引范围，会返回nullptr
    RibbonCategory *categoryByIndex(int index) const;

    //隐藏category,并不会删除或者取走，只是隐藏
    void hideCategory(RibbonCategory *category);

    //显示被隐藏的category
    void showCategory(RibbonCategory *category);

    //判断这个category是否在显示状态，也就是tabbar有这个category
    bool isCategoryVisible(RibbonCategory *category) const;

    //获取category的索引
    int categoryIndex(RibbonCategory *c) const;

    //移动一个Category从from index到to index
    void moveCategory(int from, int to);

    //获取当前显示的所有的RibbonCategory，包含未显示的RibbonContextCategory的RibbonCategory也一并返回
    QList<RibbonCategory *> categoryPages(bool getAll = true) const;

    //移除RibbonCategory
    void removeCategory(RibbonCategory *category);

    //添加一个上下文标签
    RibbonContextCategory *
    addContextCategory(const QString &title, const QColor &color = QColor(), const QVariant &id = QVariant());

    //显示一个上下文标签
    void showContextCategory(RibbonContextCategory *context);

    //隐藏一个上下文标签
    void hideContextCategory(RibbonContextCategory *context);

    //判断上下文是否是在显示状态
    bool isContextCategoryVisible(RibbonContextCategory *context);

    //设置上下文标签的显示或隐藏
    void setContextCategoryVisible(RibbonContextCategory *context, bool visible);

    //获取所有的上下文标签
    QList<RibbonContextCategory *> contextCategoryList() const;

    //移除ContextCategory
    void destroyContextCategory(RibbonContextCategory *context);

    //设置为隐藏模式
    void setMinimumMode(bool isHide);

    //当前Ribbon是否是隐藏模式
    bool isMinimumMode() const;

    //设置显示隐藏ribbon按钮
    void showMinimumModeButton(bool isShow = true);

    //是否显示隐藏ribbon按钮
    bool haveShowMinimumModeButton() const;

    //ribbon tab的高度
    int tabBarHeight() const;

    //标题栏的高度
    int titleBarHeight() const;

    //激活tabbar右边的按钮群
    RibbonButtonGroupWidget *activeTabBarRightButtonGroup();

    //快速响应栏
    RibbonQuickAccessBar *quickAccessBar();

    //设置ribbon的风格
    void setRibbonStyle(RibbonStyle v);

    //当前ribbon的风格
    RibbonStyle currentRibbonStyle() const;

    //当前的模式
    RibbonState currentRibbonState() const;

    //设置当前ribbon的index
    void setCurrentIndex(int index);

    //确保标签显示出来
    void raiseCategory(RibbonCategory *category);

    //判断当前的样式是否为两行
    bool isTwoRowStyle() const;

    //判断当前的样式是否为office样式
    bool isOfficeStyle() const;

    //告诉ribbonbar，window button的尺寸
    void setWindowButtonSize(const QSize &size);

signals:

    /**
     * @brief 应用按钮点击响应 - 左上角的按钮，通过关联此信号触发应用按钮点击的效果
     */
    void applitionButtonClicked();

    /**
     * @brief 标签页变化触发的信号
     * @param index
     */
    void currentRibbonTabChanged(int index);

protected:
    bool eventFilter(QObject *obj, QEvent *e) override;

    //根据情况重置tabbar的宽度，主要针对wps模式
    int calcMinTabBarWidth() const;

    //根据currentRibbonStyle计算mainBar的高度
    virtual int mainBarHeight() const;

    //应用按钮的宽度
    virtual int applitionButtonWidth() const;

protected slots:

    void onWindowTitleChanged(const QString &title);

    void onWindowIconChanged(const QIcon &icon);

    void onCategoryWindowTitleChanged(const QString &title);

    void onStackWidgetHided();

    virtual void onCurrentRibbonTabChanged(int index);

    virtual void onCurrentRibbonTabClicked(int index);

    virtual void onCurrentRibbonTabDoubleClicked(int index);

    void onContextsCategoryPageAdded(RibbonCategory *category);

    void onTabMoved(int from, int to);

private:
    int tabIndex(RibbonCategory *obj);

    void updateRibbonElementGeometry();

    void resizeInOfficeStyle();

    void resizeInWpsLiteStyle();

    void paintInNormalStyle();

    void paintInWpsLiteStyle();

    void resizeStackedContainerWidget();

    //刷新所有ContextCategoryManagerData，这个在单独一个Category删除时调用
    void updateContextCategoryManagerData();

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;

    virtual void paintBackground(QPainter &painter);

    virtual void paintWindowTitle(QPainter &painter, const QString &title, const QRect &titleRegion);

    virtual void paintWindowIcon(QPainter &painter, const QIcon &icon);

    virtual void
    paintContextCategoryTab(QPainter &painter, const QString &title, QRect contextRect, const QColor &color);

private:
    RibbonBarPrivate *m_d;
};


#endif //RIBBON_RIBBONBAR_H
