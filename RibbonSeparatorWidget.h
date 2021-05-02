//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONSEPARATORWIDGET_H
#define RIBBON_RIBBONSEPARATORWIDGET_H


#include <QtGui/QPaintEvent>
#include <QWidget>

class RibbonSeparatorWidget : public QWidget{
    Q_OBJECT
public:
    RibbonSeparatorWidget(int height, QWidget *parent = nullptr);
    RibbonSeparatorWidget(QWidget *parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;
    //设置分割线的上下距离
    void setTopBottomMargins(int top,int bottom);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    int m_topMargins; ///< 顶部间隔,默认4
    int m_bottomMargins; ///< 底部间隔,默认4
};


#endif //RIBBON_RIBBONSEPARATORWIDGET_H
