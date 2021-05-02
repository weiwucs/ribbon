//
// Created by weiwu on 21-4-30.
//

#include "RibbonButtonGroupWidget.h"
#include "RibbonToolButton.h"
#include "RibbonElementManager.h"
#include "RibbonSeparatorWidget.h"


class RibbonButtonGroupWidgetItem {
public:
    QAction *action;
    QWidget *widget;
    bool customWidget;

    bool operator==(QAction *action);

    bool operator==(const RibbonButtonGroupWidgetItem &w);

    RibbonButtonGroupWidgetItem();

    RibbonButtonGroupWidgetItem(QAction *a, QWidget *w, bool cw);
};


class RibbonButtonGroupWidgetPrivate {
public:
    RibbonButtonGroupWidget *Parent;
    QList<RibbonButtonGroupWidgetItem> mItems;///< 用于记录所有管理的item
    RibbonButtonGroupWidgetPrivate(RibbonButtonGroupWidget *p) {
        Parent = p;
    }


    void init() {
        QHBoxLayout *layout = new QHBoxLayout;

        layout->setMargin(0);
        layout->setSpacing(0);
        Parent->setLayout(layout);
        Parent->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    }
};


//////////////////////////////////////////////


bool RibbonButtonGroupWidgetItem::operator==(QAction *action) {
    return (this->action == action);
}


bool RibbonButtonGroupWidgetItem::operator==(const RibbonButtonGroupWidgetItem &w) {
    return (this->action == w.action);
}


RibbonButtonGroupWidgetItem::RibbonButtonGroupWidgetItem()
        : action(nullptr), widget(nullptr), customWidget(false) {
}


RibbonButtonGroupWidgetItem::RibbonButtonGroupWidgetItem(QAction *a, QWidget *w, bool cw)
        : action(a), widget(w), customWidget(cw) {
}


//////////////////////////////////////////////

RibbonButtonGroupWidget::RibbonButtonGroupWidget(QWidget *parent)
        : QFrame(parent), m_d(new RibbonButtonGroupWidgetPrivate(this)) {
    m_d->init();
}


RibbonButtonGroupWidget::~RibbonButtonGroupWidget() {
    for (RibbonButtonGroupWidgetItem &item : m_d->mItems) {
        if (QWidgetAction *widgetAction = qobject_cast<QWidgetAction *>(item.action)) {
            if (item.customWidget) {
                widgetAction->releaseWidget(item.widget);
            }
        }
    }
    delete m_d;
}


void RibbonButtonGroupWidget::addAction(QAction *a) {
    QWidget::addAction(a);
}


/**
 * @brief 生成action
 * @note action的所有权归RibbonButtonGroupWidget
 * @param text
 * @param icon
 * @param popMode
 * @return
 */
QAction *
RibbonButtonGroupWidget::addAction(const QString &text, const QIcon &icon, QToolButton::ToolButtonPopupMode popMode) {
    QAction *a = new QAction(icon, text, this);


    RibbonToolButton *btn = qobject_cast<RibbonToolButton *>(m_d->mItems.back().widget);

    if (btn) {
        btn->setPopupMode(popMode);
    }
    return (a);
}


void RibbonButtonGroupWidget::addMenu(QMenu *menu, QToolButton::ToolButtonPopupMode popMode) {
    QAction *a = menu->menuAction();

    addAction(a);
    RibbonToolButton *btn = qobject_cast<RibbonToolButton *>(m_d->mItems.back().widget);

    btn->setPopupMode(popMode);
}


QAction *RibbonButtonGroupWidget::addSeparator() {
    QAction *a = new QAction();

    a->setSeparator(true);
    addAction(a);
    return (a);
}


QAction *RibbonButtonGroupWidget::addWidget(QWidget *w) {
    QWidgetAction *a = new QWidgetAction(this);

    a->setDefaultWidget(w);
    w->setAttribute(Qt::WA_Hover);
    addAction(a);
    return (a);
}


QSize RibbonButtonGroupWidget::sizeHint() const {
    return (layout()->sizeHint());
}


QSize RibbonButtonGroupWidget::minimumSizeHint() const {
    return (layout()->minimumSize());
}


/**
 * @brief 处理action的事件
 *
 * 这里处理了ActionAdded，ActionChanged，ActionRemoved三个事件
 * ActionAdded时会生成窗口
 * @param e
 */
void RibbonButtonGroupWidget::actionEvent(QActionEvent *e) {
    RibbonButtonGroupWidgetItem item;

    item.action = e->action();
    QWidgetAction *widgetAction = qobject_cast<QWidgetAction *>(item.action);

    switch (e->type()) {
        case QEvent::ActionAdded: {
            if (nullptr != widgetAction) {
                if (widgetAction->parent() != this) {
                    widgetAction->setParent(this);
                }
            }
            if (QWidgetAction *widgetAction = qobject_cast<QWidgetAction *>(item.action)) {
                item.widget = widgetAction->requestWidget(this);
                if (item.widget != nullptr) {
                    item.widget->setAttribute(Qt::WA_LayoutUsesWidgetRect);
                    item.customWidget = true;
                }
            } else if (item.action->isSeparator()) {
                RibbonSeparatorWidget *sp = RibbonSubElementDelegate->createRibbonSeparatorWidget(this);
                sp->setTopBottomMargins(3, 3);
                item.widget = sp;
            }
            //不是widget，自动生成RibbonToolbutton
            if (!item.widget) {
                RibbonToolButton *button = RibbonSubElementDelegate->createRibbonToolButton(this);
                button->setAutoRaise(true);
                button->setFocusPolicy(Qt::NoFocus);
                button->setButtonType(RibbonToolButton::SmallButton);
                button->setToolButtonStyle(Qt::ToolButtonIconOnly);
                button->setDefaultAction(item.action);
                //根据QAction的属性设置按钮的大小

                QObject::connect(button, &RibbonToolButton::triggered, this, &RibbonButtonGroupWidget::actionTriggered);
                item.widget = button;
            }
            layout()->addWidget(item.widget);
            m_d->mItems.append(item);
        }
            break;

        case QEvent::ActionChanged: {
            //让布局重新绘制
            layout()->invalidate();
        }
            break;

        case QEvent::ActionRemoved: {
            item.action->disconnect(this);
            auto i = m_d->mItems.begin();
            for (; i != m_d->mItems.end();) {
                QWidgetAction *widgetAction = qobject_cast<QWidgetAction *>(i->action);
                if ((widgetAction != 0) && i->customWidget) {
                    widgetAction->releaseWidget(i->widget);
                } else {
                    // destroy the QToolButton/QToolBarSeparator
                    i->widget->hide();
                    i->widget->deleteLater();
                }
                i = m_d->mItems.erase(i);
            }
            layout()->invalidate();
        }
            break;

        default:
            break;
    }
}



