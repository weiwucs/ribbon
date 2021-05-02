//
// Created by weiwu on 21-5-1.
//

#include "RibbonMainWindow.h"
#include "FramelessHelper.h"
#include "WindowButtonGroup.h"

class RibbonMainWindowPrivate {
public:
    RibbonMainWindowPrivate(RibbonMainWindow *p);

    void init();

    RibbonMainWindow *Parent;
    RibbonBar *ribbonBar;
    RibbonMainWindow::RibbonTheme currentRibbonTheme;
    WindowButtonGroup *windowButtonGroup;
    FramelessHelper *pFramelessHelper;
    bool useRibbon;
};

RibbonMainWindowPrivate::RibbonMainWindowPrivate(RibbonMainWindow *p)
        : Parent(p), ribbonBar(nullptr), currentRibbonTheme(RibbonMainWindow::Office2013), windowButtonGroup(nullptr),
          pFramelessHelper(nullptr), useRibbon(true) {
}


void RibbonMainWindowPrivate::init() {
}


RibbonMainWindow::RibbonMainWindow(QWidget *parent, bool useRibbon)
        : QMainWindow(parent), m_d(new RibbonMainWindowPrivate(this)) {
    m_d->init();
    m_d->useRibbon = useRibbon;
    qDebug() << windowFlags();
    if (useRibbon) {
        qDebug() << "Ribbon v0.3.1";
        setRibbonTheme(ribbonTheme());
        //
        m_d->ribbonBar = new RibbonBar(this);

        setMenuWidget(m_d->ribbonBar);
        m_d->ribbonBar->installEventFilter(this);
        //设置窗体的标题栏高度
        m_d->pFramelessHelper = new FramelessHelper(this);
        m_d->pFramelessHelper->setTitleHeight(m_d->ribbonBar->titleBarHeight());
        //设置window按钮
        m_d->windowButtonGroup = new WindowButtonGroup(this);
        //在ribbonBar捕获windowButtonGroup，主要捕获其尺寸
        //m_d->windowButtonGroup->installEventFilter(m_d->ribbonBar);
        QSize s = m_d->windowButtonGroup->size();
        s.setHeight(m_d->ribbonBar->titleBarHeight());
        m_d->windowButtonGroup->setFixedSize(s);
        m_d->windowButtonGroup->setWindowStates(windowState());
#ifdef Q_OS_UNIX
        //某些系统会对FramelessWindowHint异常
        //FramelessHelper用如下这种setWindowFlags(w->windowFlags()|Qt::FramelessWindowHint);方式设置，改为强制取消其他
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif
    }
}


RibbonMainWindow::~RibbonMainWindow() {
    delete m_d;
}


const RibbonBar *RibbonMainWindow::ribbonBar() const {
    return (m_d->ribbonBar);
}


/**
 * @brief 如果useRibbon为false，会返回nullptr
 * @return
 */
RibbonBar *RibbonMainWindow::ribbonBar() {
    return (m_d->ribbonBar);
}


void RibbonMainWindow::setRibbonTheme(RibbonMainWindow::RibbonTheme theme) {
    switch (theme) {
        case NormalTheme:
            loadTheme(":/theme/resource/default.qss");
            break;

        case Office2013:
            loadTheme(":/theme/resource/office2013.qss");
            break;

        default:
            loadTheme(":/theme/resource/default.qss");
            break;
    }
}


RibbonMainWindow::RibbonTheme RibbonMainWindow::ribbonTheme() const {
    return (m_d->currentRibbonTheme);
}


bool RibbonMainWindow::isUseRibbon() const {
    return (m_d->useRibbon);
}


void RibbonMainWindow::resizeEvent(QResizeEvent *event) {
    if (m_d->ribbonBar) {
        if (m_d->ribbonBar->size().width() != this->size().width()) {
            m_d->ribbonBar->setFixedWidth(this->size().width());
        }
        if (m_d->windowButtonGroup) {
            m_d->ribbonBar->setWindowButtonSize(m_d->windowButtonGroup->size());
        }
    }
    QMainWindow::resizeEvent(event);
}


bool RibbonMainWindow::eventFilter(QObject *obj, QEvent *e) {
    //这个过滤是为了把ribbonBar上的动作传递到mainwindow，再传递到frameless，
    //由于ribbonbar会遮挡掉frameless的区域，导致frameless无法捕获这些消息
    if (obj == m_d->ribbonBar) {
        switch (e->type()) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease:
            case QEvent::MouseMove:
            case QEvent::Leave:
            case QEvent::HoverMove:
            case QEvent::MouseButtonDblClick:
                QApplication::sendEvent(this, e);

            default:
                break;
        }
    }
    return (QMainWindow::eventFilter(obj, e));
}


bool RibbonMainWindow::event(QEvent *e) {
    if (e) {
        switch (e->type()) {
            case QEvent::WindowStateChange: {
                if (isUseRibbon()) {
                    m_d->windowButtonGroup->setWindowStates(windowState());
                }
            }
                break;

            default:
                break;
        }
    }
    return (QMainWindow::event(e));
}


void RibbonMainWindow::loadTheme(const QString &themeFile) {
    QFile file(themeFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    setStyleSheet(file.readAll());
}


