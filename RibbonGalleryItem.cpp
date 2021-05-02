//
// Created by weiwu on 21-4-30.
//

#include "RibbonGalleryItem.h"
#include "RibbonGalleryGroup.h"

RibbonGalleryItem::RibbonGalleryItem()
        : m_flsgs(Qt::ItemIsEnabled | Qt::ItemIsSelectable), m_action(nullptr) {

}

RibbonGalleryItem::RibbonGalleryItem(const QIcon &icon)
        : m_flsgs(Qt::ItemIsEnabled | Qt::ItemIsSelectable), m_action(nullptr) {
    setIcon(icon);
}

RibbonGalleryItem::RibbonGalleryItem(QAction *act)
        : m_flsgs(Qt::ItemIsEnabled | Qt::ItemIsSelectable) {
    setAction(act);
}

RibbonGalleryItem::~RibbonGalleryItem() {

}


void RibbonGalleryItem::setData(int role, const QVariant &data) {
    m_datas[role] = data;
}

QVariant RibbonGalleryItem::data(int role) const {
    if (m_action) {
        switch (role) {
            case Qt::DisplayRole:
                return m_action->text();
            case Qt::ToolTipRole:
                return m_action->toolTip();
            case Qt::DecorationRole:
                return m_action->icon();
            default:
                break;
        }
    }
    return m_datas.value(role);
}

void RibbonGalleryItem::setText(const QString &text) {
    setData(Qt::DisplayRole, text);
}

QString RibbonGalleryItem::text() const {
    if (m_action) {
        return m_action->text();
    }
    return data(Qt::DisplayRole).toString();
}

void RibbonGalleryItem::setToolTip(const QString &text) {
    setData(Qt::ToolTipRole, text);
}

QString RibbonGalleryItem::toolTip() const {
    if (m_action) {
        return m_action->toolTip();
    }
    return data(Qt::ToolTipRole).toString();
}

void RibbonGalleryItem::setIcon(const QIcon &ico) {
    setData(Qt::DecorationRole, ico);
}

QIcon RibbonGalleryItem::icon() const {
    if (m_action) {
        return m_action->icon();
    }
    return qvariant_cast<QIcon>(data(Qt::DecorationRole));
}

bool RibbonGalleryItem::isSelectable() const {
    return (m_flsgs & Qt::ItemIsSelectable);
}

void RibbonGalleryItem::setSelectable(bool isSelectable) {
    if (isSelectable) {
        m_flsgs |= Qt::ItemIsSelectable;
    } else {
        m_flsgs = (m_flsgs & (~Qt::ItemIsSelectable));
    }
}

bool RibbonGalleryItem::isEnable() const {
    if (m_action) {
        return m_action->isEnabled();
    }
    return (m_flsgs & Qt::ItemIsEnabled);
}

void RibbonGalleryItem::setEnable(bool isEnable) {
    if (m_action) {
        m_action->setEnabled(isEnable);
    }

    if (isEnable) {
        m_flsgs |= Qt::ItemIsEnabled;
    } else {
        m_flsgs = (m_flsgs & (~Qt::ItemIsEnabled));
    }
}

void RibbonGalleryItem::setFlags(Qt::ItemFlags flag) {
    m_flsgs = flag;
    if (m_action) {
        m_action->setEnabled(flag & Qt::ItemIsEnabled);
    }
}

Qt::ItemFlags RibbonGalleryItem::flags() const {
    return m_flsgs;
}

void RibbonGalleryItem::setAction(QAction *act) {
    m_action = act;
    if (act->isEnabled()) {
        m_flsgs |= Qt::ItemIsEnabled;
    } else {
        m_flsgs = (m_flsgs & (~Qt::ItemIsEnabled));
    }
}

QAction *RibbonGalleryItem::action() {
    return m_action;
}
