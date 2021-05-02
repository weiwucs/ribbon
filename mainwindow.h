//
// Created by weiwu on 21-5-2.
//

#ifndef RIBBON_MAINWINDOW_H
#define RIBBON_MAINWINDOW_H


#include "RibbonMainWindow.h"
#include "RibbonCustomizeWidget.h"

class mainwindow : public RibbonMainWindow{
Q_OBJECT
public:
    mainwindow(QWidget *par = nullptr);
private slots:
    void onShowContextCategory(bool on);
    void onStyleClicked(int id);


private:
    void createCategoryMain(RibbonCategory *page);
    void createCategoryOther(RibbonCategory *page);
    void createContextCategoryPage1(RibbonCategory *page);
    void createContextCategoryPage2(RibbonCategory *page);
    void addSomeOtherAction();

private slots:
    void onMenuButtonPopupCheckableTest(bool b);
    void onInstantPopupCheckableTest(bool b);
    void onDelayedPopupCheckableTest(bool b);

    void onMenuButtonPopupCheckabletriggered(bool b);
    void onInstantPopupCheckabletriggered(bool b);
    void onDelayedPopupCheckabletriggered(bool b);

private:
    RibbonContextCategory *m_contextCategory;
    RibbonCustomizeWidget *m_customizeWidget;
    QTextEdit *m_edit;
    RibbonActionsManager *m_actMgr;
    int m_actionTagText;
    int m_actionTagWithIcon;
};


#endif //RIBBON_MAINWINDOW_H
