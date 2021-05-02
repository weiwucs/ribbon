//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONTABBAR_H
#define RIBBON_RIBBONTABBAR_H


#include <QtCore/QMargins>
#include <QtCore/QArgument>
#include <QtWidgets/QTabBar>

class RibbonTabBar : public QTabBar{
    Q_OBJECT
public:
    RibbonTabBar(QWidget *parent = Q_NULLPTR);
    const QMargins& tabMargin() const;
    void setTabMargin(const QMargins& tabMargin);

private:
    QMargins m_tabMargin;
};


#endif //RIBBON_RIBBONTABBAR_H
