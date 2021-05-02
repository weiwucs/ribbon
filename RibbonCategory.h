//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONCATEGORY_H
#define RIBBON_RIBBONCATEGORY_H

#include <QtWidgets/QWidget>
#include "RibbonPannel.h"
#include <QWheelEvent>

class RibbonCategoryProxyPrivate;

class RibbonCategoryPrivate;
class QHBoxLayout;
class QWheelEvent;

/**
 * @brief 一项ribbon tab页
 * @note RibbonCategory的windowTitle影响了其在RibbonBar的标签显示，
 * 如果要改标签名字，直接调用RibbonCategory的setWindowTitle函数
 */
class RibbonCategory : public QWidget {
Q_OBJECT

    friend class RibbonBar;

    friend class RibbonContextCategory;

    Q_PROPERTY(bool isCanCustomize
                       READ
                       isCanCustomize
                       WRITE
                       setCanCustomize)
public:
    RibbonCategory(QWidget *parent);

    ~RibbonCategory();

    //category的名字
    QString categoryName() const;

    //设置category名字，等同setWindowTitle
    void setCategoryName(const QString &title);

    //布局模式
    RibbonPannel::PannelLayoutMode ribbonPannelLayoutMode() const;

    //添加pannel
    RibbonPannel *addPannel(const QString &title);

    //插入pannel
    RibbonPannel *insertPannel(const QString &title, int index);

    //通过名字查找pannel
    RibbonPannel *pannelByName(const QString &title) const;

    //通过ObjectName查找pannel
    RibbonPannel *pannelByObjectName(const QString &objname) const;

    //通过索引找到pannel，如果超过索引范围，会返回nullptr
    RibbonPannel *pannelByIndex(int index) const;

    //查找pannel的index
    int pannelIndex(RibbonPannel *p) const;

    //移动一个Pannel从from index到to index
    void movePannel(int from, int to);

    //添加pannel
    void addPannel(RibbonPannel *pannel);

    //把pannel从Category中移除，不会销毁，此时pannel的所有权归还操作者
    bool takePannel(RibbonPannel *pannel);

    //移除Pannel，Category会直接回收RibbonPannel内存
    bool removePannel(RibbonPannel *pannel);

    //设置背景
    void setBackgroundBrush(const QBrush &brush);

    //返回所有的Pannel
    QList<RibbonPannel *> pannelList() const;

    //
    QSize sizeHint() const Q_DECL_OVERRIDE;

    //如果是ContextCategory，此函数返回true
    bool isContextCategory() const;

    //pannel的个数
    int pannelCount() const;

    //判断是否可以自定义
    bool isCanCustomize() const;

    void setCanCustomize(bool b);

protected slots:

    void onLeftScrollButtonClicked();

    void onRightScrollButtonClicked();

protected:
    //事件处理
    bool event(QEvent *e) Q_DECL_OVERRIDE;

    //
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

    //设置pannel的模式
    void setRibbonPannelLayoutMode(RibbonPannel::PannelLayoutMode m);

    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

    //处理滚轮事件
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    //标记这个是上下文标签
    void markIsContextCategory(bool isContextCategory = true);

private:
    RibbonCategoryPrivate *m_d;
};

/**
 * @brief RibbonCategory无法完全显示时，显示的调整按钮
 *
 * 重新定义是为了防止被外部的样式影响,同时可以使用RibbonCategoryScrollButton的样式定义
 */
class RibbonCategoryScrollButton : public QToolButton {
Q_OBJECT
public:
    using QToolButton::QToolButton;

    RibbonCategoryScrollButton(Qt::ArrowType arr, QWidget *p = nullptr);
};


#endif //RIBBON_RIBBONCATEGORY_H
