//
// Created by weiwu on 21-5-1.
//

#include <QResizeEvent>
#include <QToolButton>
#include <QStyle>
#include <QDebug>
#include <QScopedPointer>
#include "WindowButtonGroup.h"

class WindowButtonGroupPrivate {
public:
    WindowButtonGroup *q_d;
    WindowToolButton *buttonClose;
    WindowToolButton *buttonMinimize;
    WindowToolButton *buttonMaximize;
    int mCloseStretch;
    int mMaxStretch;
    int mMinStretch;
    qreal mIconscale;
    QScopedPointer<Qt::WindowFlags> mFlags;

    WindowButtonGroupPrivate(WindowButtonGroup *p)
            : q_d(p), buttonClose(nullptr), buttonMinimize(nullptr), buttonMaximize(nullptr), mCloseStretch(4),
              mMaxStretch(3), mMinStretch(3), mIconscale(0.5) {
    }


    void setupMinimizeButton(bool on) {
        WindowButtonGroup *par = q_d;

        if (on) {
            if (buttonMinimize) {
                buttonMinimize->deleteLater();
            }
            buttonMinimize = new WindowToolButton(par);
            buttonMinimize->setObjectName("MinimizeWindowButton");
            buttonMinimize->setFixedSize(30, 28);
            buttonMinimize->setFocusPolicy(Qt::NoFocus);//避免铺抓到
            buttonMinimize->setStyleSheet(QString(""
                                                  "WindowToolButton"
                                                  "{ "
                                                  "    background-color: transparent; "
                                                  "    border:none;"
                                                  "}"
                                                  "WindowToolButton:hover {background-color:#C1D1B8;}"
                                                  "WindowToolButton:pressed {background-color:#A5AF9B;}"
                                                  "WindowToolButton:focus{outline: none}"));
            QIcon icon = par->style()->standardIcon(QStyle::SP_TitleBarMinButton);
            buttonMinimize->setIconSize(buttonMinimize->size() * mIconscale);
            buttonMinimize->setIcon(icon);
            par->connect(buttonMinimize, &QAbstractButton::clicked, par, &WindowButtonGroup::minimizeWindow);
        } else {
            if (buttonMinimize) {
                delete buttonMinimize;
            }
        }
        updateSize();
    }


    void setupMaximizeButton(bool on) {
        WindowButtonGroup *par = q_d;

        if (on) {
            if (buttonMaximize) {
                buttonMaximize->deleteLater();
            }
            buttonMaximize = new WindowToolButton(par);
            buttonMaximize->setObjectName("MaximizeWindowButton");
            buttonMaximize->setFixedSize(30, 28);
            buttonMaximize->setFocusPolicy(Qt::NoFocus);//避免铺抓到
            buttonMaximize->setStyleSheet(QString(""
                                                  "WindowToolButton"
                                                  "{ "
                                                  "    background-color: transparent; "
                                                  "    border:none;"
                                                  "}\n"
                                                  "WindowToolButton:hover {background-color:#C1D1B8;}"
                                                  "WindowToolButton:pressed {background-color:#A5AF9B;}"
                                                  "WindowToolButton:focus{outline: none;}"));
            QIcon icon = par->style()->standardIcon(QStyle::SP_TitleBarMaxButton);
            buttonMaximize->setIconSize(buttonMaximize->size() * mIconscale);
            buttonMaximize->setIcon(icon);
            par->connect(buttonMaximize, &QAbstractButton::clicked, par, &WindowButtonGroup::maximizeWindow);
        } else {
            if (buttonMaximize) {
                delete buttonMaximize;
            }
        }
        updateSize();
    }


    void setupCloseButton(bool on) {
        WindowButtonGroup *par = q_d;

        if (on) {
            if (buttonClose) {
                buttonClose->deleteLater();
            }
            buttonClose = new WindowToolButton(par);
            buttonClose->setObjectName("CloseWindowButton");
            buttonClose->setFixedSize(40, 28);
            buttonClose->setFocusPolicy(Qt::NoFocus);//避免铺抓到
            //buttonClose->setFlat(true);
            par->connect(buttonClose, &QAbstractButton::clicked, par, &WindowButtonGroup::closeWindow);
            buttonClose->setStyleSheet(QString(""
                                               "WindowToolButton"
                                               "{ "
                                               "    background-color: transparent; "
                                               "    border:none;"
                                               "}"
                                               "WindowToolButton:hover {background-color:#F0604D;}"
                                               "WindowToolButton:pressed {background-color:#F0604D;}"
                                               "WindowToolButton:focus{outline: none;}"));
            QIcon icon = par->style()->standardIcon(QStyle::SP_TitleBarCloseButton);
            buttonClose->setIconSize(buttonClose->size() * mIconscale);
            buttonClose->setIcon(icon);
        } else {
            if (buttonClose) {
                delete buttonClose;
            }
        }
        updateSize();
    }


    void updateSize() {
        q_d->setFixedSize(sizeHint());
        resize(q_d->size());
    }


    void resize(QSize size) {
        qreal tw = 0;

        if (buttonClose) {
            tw += mCloseStretch;
        }
        if (buttonMaximize) {
            tw += mMaxStretch;
        }
        if (buttonMinimize) {
            tw += mMinStretch;
        }
        //调整按钮

        int x = 0;

        if (buttonMinimize) {
            int w = (mMinStretch / tw) * size.width();
            buttonMinimize->setGeometry(x, 0, w, size.height());
            x += w;
        }
        if (buttonMaximize) {
            int w = (mMaxStretch / tw) * size.width();
            buttonMaximize->setGeometry(x, 0, w, size.height());
            x += w;
        }
        if (buttonClose) {
            int w = (mCloseStretch / tw) * size.width();
            buttonClose->setGeometry(x, 0, w, size.height());
        }
    }


    QSize sizeHint() const {
        int width = 0;
        int height = 28;

        if (buttonClose) {
            width += 40;
        }
        if (buttonMaximize) {
            width += 30;
        }
        if (buttonMinimize) {
            width += 30;
        }
        return (QSize(width, height));
    }
};

WindowToolButton::WindowToolButton(QWidget *p) : QToolButton(p) {
    setAutoRaise(true);
}


WindowButtonGroup::WindowButtonGroup(QWidget *parent) : QWidget(parent), m_d(new WindowButtonGroupPrivate(this)) {
    updateWindowFlag();
    if (parent) {
        parent->installEventFilter(this);
    }
}


/**
 * @brief 构造函数，强制使用flags，而不是用parent的flags进行构造
 * @param parent
 * @param flags
 */
WindowButtonGroup::WindowButtonGroup(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent),
                                                                               m_d(new WindowButtonGroupPrivate(
                                                                                       this)) {
    m_d->mFlags.reset(new Qt::WindowFlags);
    *m_d->mFlags = flags;
    updateWindowFlag();
    if (parent) {
        parent->installEventFilter(this);
    }
}


WindowButtonGroup::~WindowButtonGroup() {
    delete m_d;
}


void WindowButtonGroup::setupMinimizeButton(bool on) {
    m_d->setupMinimizeButton(on);
}


void WindowButtonGroup::setupMaximizeButton(bool on) {
    m_d->setupMaximizeButton(on);
}


void WindowButtonGroup::setupCloseButton(bool on) {
    m_d->setupCloseButton(on);
}


void WindowButtonGroup::updateWindowFlag() {
    Qt::WindowFlags flags = parentWidget()->windowFlags();

    if (!m_d->mFlags.isNull()) {
        flags = *(m_d->mFlags);
    }

    setupMinimizeButton(flags & Qt::WindowMinimizeButtonHint);

    setupMaximizeButton(flags & Qt::WindowMaximizeButtonHint);

    setupCloseButton(flags & Qt::WindowCloseButtonHint);
}


/**
 * @brief 设置按钮的宽度比例,最终按钮宽度将按照此比例进行设置
 * @param close 关闭按钮比例
 * @param max 最大化按钮比例
 * @param min 最小化按钮比例
 */
void WindowButtonGroup::setButtonWidthStretch(int close, int max, int min) {
    m_d->mMaxStretch = max;
    m_d->mMinStretch = min;
    m_d->mCloseStretch = close;
}


/**
 * @brief 设置按钮的缩放比例
 * @param iconscale 数值越小按钮越小
 */
void WindowButtonGroup::setIconScale(qreal iconscale) {
    m_d->mIconscale = iconscale;
}


/**
 * @brief 设置窗口状态（最大最小化按钮状态）
 * @param s
 */
void WindowButtonGroup::setWindowStates(Qt::WindowStates s) {
    switch (s) {
        case Qt::WindowNoState:
            if (m_d->buttonMaximize) {
                m_d->buttonMaximize->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
            }
            break;

        case Qt::WindowMaximized:
            if (m_d->buttonMaximize) {
                m_d->buttonMaximize->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
            }
            break;

        default:
            break;
    }
}


QSize WindowButtonGroup::sizeHint() const {
    return (m_d->sizeHint());
}


bool WindowButtonGroup::eventFilter(QObject *watched, QEvent *e) {
    //用于监听父窗口改变尺寸
    if (watched == parentWidget()) {
        switch (e->type()) {
            case QEvent::Resize:
                parentResize();
                break;

            default:
                break;
        }
    }
    return (false);//不截断任何事件
}


void WindowButtonGroup::parentResize() {
    QWidget *par = parentWidget();

    if (par) {
        QSize parSize = par->size();
        move(parSize.width() - width() - 1, 1);
    }
}


void WindowButtonGroup::resizeEvent(QResizeEvent *e) {
    m_d->resize(e->size());
}


void WindowButtonGroup::closeWindow() {
    if (parentWidget()) {
        parentWidget()->close();
    }
}


void WindowButtonGroup::minimizeWindow() {
    if (parentWidget()) {
        parentWidget()->showMinimized();
    }
}


void WindowButtonGroup::maximizeWindow() {
    QWidget *par = parentWidget();

    if (par) {
        if (par->isMaximized()) {
            par->showNormal();
        } else {
            par->showMaximized();
        }
    }
}
