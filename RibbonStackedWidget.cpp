//
// Created by weiwu on 21-5-1.
//

#include <QtCore/QEventLoop>
#include <QResizeEvent>
#include "RibbonStackedWidget.h"


class RibbonStackedWidgetPrivate {
public:
    RibbonStackedWidget *Parent;
    QEventLoop *eventLoop;
    bool iutoResize;

    RibbonStackedWidgetPrivate(RibbonStackedWidget *p)
            : Parent(p), eventLoop(nullptr), iutoResize(true) {
    }


    void init() {
        //Parent->setFocusPolicy(Qt::StrongFocus);
    }
};


RibbonStackedWidget::RibbonStackedWidget(QWidget *parent)
        : QStackedWidget(parent), m_d(new RibbonStackedWidgetPrivate(this)) {
    m_d->init();
    setNormalMode();
}


RibbonStackedWidget::~RibbonStackedWidget() {
    if (m_d->eventLoop) {
        m_d->eventLoop->exit();
    }
    delete m_d;
}


void RibbonStackedWidget::setPopupMode() {
    setMouseTracking(true);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setFrameShape(QFrame::Panel);
}


bool RibbonStackedWidget::isPopupMode() const {
    return (windowFlags() & Qt::Popup);
}


void RibbonStackedWidget::setNormalMode() {
    if (m_d->eventLoop) {
        m_d->eventLoop->exit();
        m_d->eventLoop = nullptr;
    }
    setMouseTracking(false);
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setFrameShape(QFrame::NoFrame);
}


bool RibbonStackedWidget::isNormalMode() const {
    return (!isPopupMode());
}


void RibbonStackedWidget::exec() {
    show();
    if (!isPopupMode()) {
        m_d->eventLoop = nullptr;
        return;
    }
    QEventLoop event;

    m_d->eventLoop = &event;
    event.exec();
    m_d->eventLoop = nullptr;
}


/**
 * @brief 设置stacked管理的窗口会随着stacked的大小变化而变化大小
 *
 * 默认为true
 * @param autoresize
 */
void RibbonStackedWidget::setAutoResize(bool autoresize) {
    m_d->iutoResize = autoresize;
}


bool RibbonStackedWidget::isAutoResize() const {
    return (m_d->iutoResize);
}


/**
 * @brief 类似tabbar的moveTab函数，交换两个窗口的index
 * @param from
 * @param to
 * @note 此操作会触发widgetRemoved(int index)信号
 */
void RibbonStackedWidget::moveWidget(int from, int to) {
    QWidget *w = widget(from);

    removeWidget(w);
    insertWidget(to, w);
}


void RibbonStackedWidget::hideEvent(QHideEvent *e) {
    if (isPopupMode()) {
        if (m_d->eventLoop) {
            m_d->eventLoop->exit();
        }
    }
    setFocus();
    emit hidWindow();

    QStackedWidget::hideEvent(e);
}


void RibbonStackedWidget::resizeEvent(QResizeEvent *event) {
    if (m_d->iutoResize) {
        int c = count();
        for (int i = 0; i < c; ++i) {
            QWidget *w = widget(i);
            if (w) {
                w->resize(event->size());
            }
        }
    }
    return (QStackedWidget::resizeEvent(event));
}
