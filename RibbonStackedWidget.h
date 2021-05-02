//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_RIBBONSTACKEDWIDGET_H
#define RIBBON_RIBBONSTACKEDWIDGET_H


#include <QtWidgets/QStackedWidget>

class RibbonStackedWidgetPrivate;

class RibbonStackedWidget : public QStackedWidget{
Q_OBJECT
public:
    RibbonStackedWidget(QWidget *parent);
    ~RibbonStackedWidget();
    void setPopupMode();
    bool isPopupMode() const;
    void setNormalMode();
    bool isNormalMode() const;
    void exec();

    //设置stacked管理的窗口会随着stacked的大小变化而变化大小
    void setAutoResize(bool autoresize);
    bool isAutoResize() const;
    void moveWidget(int from, int to);

protected:
//    void mouseReleaseEvent(QMouseEvent *e);
    void hideEvent(QHideEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void hidWindow();

private:
    RibbonStackedWidgetPrivate *m_d;
};


#endif //RIBBON_RIBBONSTACKEDWIDGET_H
