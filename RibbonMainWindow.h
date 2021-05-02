//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONMAINWINDOW_H
#define RIBBON_RIBBONMAINWINDOW_H


#include <QtWidgets/QMainWindow>
#include "RibbonBar.h"


class RibbonMainWindowPrivate;

class RibbonMainWindow : public QMainWindow{
Q_OBJECT
public:
    RibbonMainWindow(QWidget *parent = nullptr, bool useRibbon = true);
    ~RibbonMainWindow();
    //返回SARibbonBar
    const RibbonBar *ribbonBar() const;
    RibbonBar *ribbonBar();

    enum RibbonTheme {
        NormalTheme     ///< 普通主题
        , Office2013    ///< office2013主题
    };
    void setRibbonTheme(RibbonTheme theme);
    RibbonTheme ribbonTheme() const;

    //判断当前是否使用ribbon模式
    bool isUseRibbon() const;

protected:
    void loadTheme(const QString& themeFile);
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *obj, QEvent *e) Q_DECL_OVERRIDE;
    virtual bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
    RibbonMainWindowPrivate *m_d;
};


#endif //RIBBON_RIBBONMAINWINDOW_H
