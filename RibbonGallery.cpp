//
// Created by weiwu on 21-4-30.
//

#include "RibbonGallery.h"
#include "RibbonMenu.h"
#include "RibbonControlButton.h"
#include "RibbonElementManager.h"

#include <QDebug>
#include <QIcon>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QApplication>

#define ICON_ARROW_UP QIcon("./resource/ArrowUp.png")
#define ICON_ARROW_DOWN QIcon("./resource/ArrowDown.png")
#define ICON_ARROW_MORE QIcon("./resource/ArrowMore.png")

class RibbonGalleryPrivate {
public:
    RibbonControlButton *buttonUp;
    RibbonControlButton *buttonDown;
    RibbonControlButton *buttonMore;
    RibbonGallery *Parent;
#if 0
    RibbonMenu* popupWidget;
#else
    RibbonGalleryViewport *popupWidget;
#endif
    RibbonGalleryGroup *viewportGroup;

    RibbonGalleryPrivate() : Parent(nullptr) {
    }

    void init(RibbonGallery *parent) {
        Parent = parent;
        buttonUp = new RibbonControlButton(parent);
        buttonDown = new RibbonControlButton(parent);
        buttonMore = new RibbonControlButton(parent);
        buttonUp->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonDown->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonMore->setToolButtonStyle(Qt::ToolButtonIconOnly);
        buttonUp->setObjectName("RibbonGalleryButtonUp");
        buttonDown->setObjectName("RibbonGalleryButtonDown");
        buttonMore->setObjectName("RibbonGalleryButtonMore");
        buttonUp->setFixedSize(15, 20);
        buttonDown->setFixedSize(15, 20);
        buttonMore->setFixedSize(15, 20);
        buttonUp->setIcon(ICON_ARROW_UP);
        buttonDown->setIcon(ICON_ARROW_DOWN);
        buttonMore->setIcon(ICON_ARROW_MORE);
        Parent->connect(buttonUp, &QAbstractButton::clicked, Parent, &RibbonGallery::onPageUp);
        Parent->connect(buttonDown, &QAbstractButton::clicked, Parent, &RibbonGallery::onPageDown);
        Parent->connect(buttonMore, &QAbstractButton::clicked, Parent, &RibbonGallery::onShowMoreDetail);
        popupWidget = nullptr;
        viewportGroup = nullptr;
    }

    bool isValid() const {
        return Parent != nullptr;
    }

    void createPopupWidget() {
        if (nullptr == popupWidget) {
#if 0
            popupWidget = new RibbonMenu(Parent);
#else
            popupWidget = new RibbonGalleryViewport(Parent);
#endif
        }
    }

    void setViewPort(RibbonGalleryGroup *v) {
        if (nullptr == viewportGroup) {
            viewportGroup = RibbonSubElementDelegate->createRibbonGalleryGroup(Parent);
        }
        viewportGroup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        viewportGroup->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        viewportGroup->setModel(v->model());
        viewportGroup->setEnableIconText(v->enableIconText());
        viewportGroup->show();
    }
};


//////////////////////////////////////////////


RibbonGalleryViewport::RibbonGalleryViewport(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Popup);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setMargin(1);
}

void RibbonGalleryViewport::addWidget(QWidget *w) {
    m_layout->addWidget(w);
}

//////////////////////////////////////////////

RibbonGallery::RibbonGallery(QWidget *parent) : QFrame(parent), m_d(new RibbonGalleryPrivate) {
    m_d->init(this);
    setFrameShape(QFrame::Box);
    //setFrameShape(QFrame::Panel);
    setFixedHeight(60);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumWidth(88);
}

RibbonGallery::~RibbonGallery() {
    delete m_d;
}

QSize RibbonGallery::sizeHint() const {
    return QSize(232, 60);
}

QSize RibbonGallery::minimumSizeHint() const {
    return QSize(88, 60);
}

RibbonGalleryGroup *RibbonGallery::addGalleryGroup() {
    RibbonGalleryGroup *group = RibbonSubElementDelegate->createRibbonGalleryGroup(this);
    RibbonGalleryViewport *viewport = ensureGetPopupViewPort();
    RibbonGalleryGroupModel *model = new RibbonGalleryGroupModel(this);
    group->setModel(model);
    viewport->addWidget(group);
    if (nullptr == m_d->viewportGroup) {
        setCurrentViewGroup(group);
    }
    connect(group, &QAbstractItemView::clicked, this, &RibbonGallery::onItemClicked);
    return group;
}

RibbonGalleryGroup *RibbonGallery::addCategoryActions(const QString &title, QList<QAction *> actions) {
    RibbonGalleryGroup *group = RibbonSubElementDelegate->createRibbonGalleryGroup(this);
    RibbonGalleryGroupModel *model = new RibbonGalleryGroupModel(this);
    group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    group->setModel(model);
    if (!title.isEmpty()) {
        group->setGroupTitle(title);
    }
    group->addActionItemList(actions);
    connect(group, &QAbstractItemView::clicked, this, &RibbonGallery::onItemClicked);
    RibbonGalleryViewport *viewport = ensureGetPopupViewPort();
    viewport->addWidget(group);
    return group;
}

void RibbonGallery::setCurrentViewGroup(RibbonGalleryGroup *group) {
    m_d->setViewPort(group);
    QApplication::postEvent(this, new QResizeEvent(size(), size()));
}

RibbonGalleryGroup *RibbonGallery::currentViewGroup() const {
    return m_d->viewportGroup;
}

void RibbonGallery::onPageDown() {
    if (m_d->viewportGroup) {
        QScrollBar *vscrollBar = m_d->viewportGroup->verticalScrollBar();
        int v = vscrollBar->value();
        v += vscrollBar->singleStep();
        vscrollBar->setValue(v);
    }
}

void RibbonGallery::onPageUp() {
    if (m_d->viewportGroup) {
        QScrollBar *vscrollBar = m_d->viewportGroup->verticalScrollBar();
        int v = vscrollBar->value();
        v -= vscrollBar->singleStep();
        vscrollBar->setValue(v);
    }
}

void RibbonGallery::onShowMoreDetail() {
    if (nullptr == m_d->popupWidget) {
        return;
    }
    QSize popupMenuSize = m_d->popupWidget->minimumSizeHint();// sizeHint();
    QPoint start = mapToGlobal(QPoint(0, 0));
    m_d->popupWidget->setGeometry(start.x(), start.y(), width(), popupMenuSize.height());
    m_d->popupWidget->show();
}

void RibbonGallery::onItemClicked(const QModelIndex &index) {
    QObject *obj = sender();
    RibbonGalleryGroup *group = qobject_cast<RibbonGalleryGroup *>(obj);

    if (group) {
        RibbonGalleryGroup *curGroup = currentViewGroup();
        if (nullptr == curGroup) {
            setCurrentViewGroup(group);
            curGroup = currentViewGroup();
        }
        if (curGroup->model() != group->model()) {
            curGroup->setModel(group->model());
        }
        curGroup->scrollTo(index);
        curGroup->setCurrentIndex(index);
        curGroup->repaint();
    }
}

RibbonGalleryViewport *RibbonGallery::ensureGetPopupViewPort() {
    if (nullptr == m_d->popupWidget) {
        m_d->createPopupWidget();
    }
    return m_d->popupWidget;
}

void RibbonGallery::resizeEvent(QResizeEvent *event) {
    if (!m_d->isValid()) {
        return;
    }
    const QSize r = event->size();
    int subW = 0;
    m_d->buttonUp->move(r.width() - m_d->buttonUp->width(), 0);
    subW = qMax(subW, m_d->buttonUp->width());
    m_d->buttonDown->move(r.width() - m_d->buttonDown->width(), m_d->buttonUp->height());
    subW = qMax(subW, m_d->buttonDown->width());
    m_d->buttonMore->move(r.width() - m_d->buttonMore->width(), m_d->buttonDown->geometry().bottom());
    subW = qMax(subW, m_d->buttonMore->width());
    if (m_d->viewportGroup) {
        m_d->viewportGroup->setGeometry(0, 0, r.width() - subW, r.height());
    }
    QFrame::resizeEvent(event);
}

void RibbonGallery::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);

}


