//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONCONTEXTCATEGORY_H
#define RIBBON_RIBBONCONTEXTCATEGORY_H


#include <QtCore/QObject>
#include "RibbonCategory.h"

class RibbonContextCategoryPrivate;

/**
 * 管理上下文标签的类
 */

class RibbonContextCategory : public QObject{
Q_OBJECT
public:
    RibbonContextCategory(QWidget *parent = 0);
    ~RibbonContextCategory();
    //上下文目录添加下属目录
    RibbonCategory *addCategoryPage(const QString& title);

    //获取上下文标签下管理的标签个数
    int categoryCount() const;

    //设置id
    void setId(const QVariant& id);
    QVariant id() const;

    //设置上下文颜色
    void setContextColor(const QColor color);
    QColor contextColor() const;

    //上下文标签的内容
    QString contextTitle() const;
    void setContextTitle(const QString& contextTitle);

    //获取对应的tab页
    RibbonCategory *categoryPage(int index);

    //获取所有的RibbonCategory*
    QList<RibbonCategory *> categoryList() const;

    //移除category
    bool takeCategory(RibbonCategory *category);

signals:
    void categoryPageAdded(RibbonCategory *category);
    void categoryPageRemoved(RibbonCategory *category);

protected:
    //获取父级窗口
    QWidget *parentWidget() const;
    virtual bool eventFilter(QObject *watched, QEvent *e) override;

private:
    RibbonContextCategoryPrivate *m_d;
};


#endif //RIBBON_RIBBONCONTEXTCATEGORY_H
