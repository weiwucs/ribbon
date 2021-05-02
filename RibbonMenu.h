//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONMENU_H
#define RIBBON_RIBBONMENU_H


#include <QtWidgets/QMenu>

class RibbonMenu : public QMenu {
    Q_OBJECT
public:
    explicit RibbonMenu(QWidget *parent = Q_NULLPTR);
    explicit RibbonMenu(const QString &title, QWidget *parent = Q_NULLPTR);

    QAction *addRibbonMenu(RibbonMenu *menu);
    RibbonMenu *addRibbonMenu(const QString &title);
    RibbonMenu *addRibbonMenu(const QIcon &icon, const QString &title);
    QAction *addWidget(QWidget* w);
};

#endif //RIBBON_RIBBONMENU_H
