//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONCTRLCONTAINER_H
#define RIBBON_RIBBONCTRLCONTAINER_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QStyleOption>
class RibbonCtrlContainerPrivate;

class RibbonCtrlContainer : public QWidget{
    Q_OBJECT
public:
    RibbonCtrlContainer(QWidget* containerWidget, QWidget *parent = Q_NULLPTR);
    ~RibbonCtrlContainer();
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QWidget *containerWidget();
    const QWidget *containerWidget() const;
    void setEnableShowIcon(bool b);
    void setEnableShowTitle(bool b);

protected:
    void setContainerWidget(QWidget *w);
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    virtual void initStyleOption(QStyleOption *opt) = 0;

private:
    RibbonCtrlContainerPrivate *m_d;
};


#endif //RIBBON_RIBBONCTRLCONTAINER_H
