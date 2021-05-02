//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONBUTTONGROUPWIDGET_H
#define RIBBON_RIBBONBUTTONGROUPWIDGET_H


#include <QtCore/QArgument>
#include <QtWidgets/QAction>
#include <QtWidgets/QtWidgets>

class RibbonButtonGroupWidgetPrivate;

class RibbonButtonGroupWidget : public QFrame {
    Q_OBJECT
public:
    RibbonButtonGroupWidget(QWidget *parent = Q_NULLPTR);

    ~RibbonButtonGroupWidget();

    //生成并添加一个action
    void addAction(QAction *a);

    QAction *addAction(const QString &text, const QIcon &icon,
                       QToolButton::ToolButtonPopupMode popMode = QToolButton::InstantPopup);

    void addMenu(QMenu *menu, QToolButton::ToolButtonPopupMode popMode = QToolButton::InstantPopup);

    QAction *addSeparator();

    QAction *addWidget(QWidget *w);

    QSize sizeHint() const;

    QSize minimumSizeHint() const ;

    signals:

            /**
             * @brief 参考QToolBar::actionTriggered的信号
             * @param action
             */
     void actionTriggered(QAction *action);

protected:
    virtual void actionEvent(QActionEvent *e);

private:
    RibbonButtonGroupWidgetPrivate *m_d;
};


#endif //RIBBON_RIBBONBUTTONGROUPWIDGET_H
