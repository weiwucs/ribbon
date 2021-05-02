//
// Created by weiwu on 21-4-30.
//

#include "RibbonGalleryGroup.h"
#include <QPainter>
#include <QDebug>

class RibbonGalleryGroupPrivate {
public:
    RibbonGalleryGroup *Parent;
    bool enableIconText;
    QString groupTitle;

    RibbonGalleryGroupPrivate(RibbonGalleryGroup *p)
            : Parent(p), enableIconText(false) {

    }
};


////////////////////////////////////////

RibbonGalleryGroupItemDelegate::RibbonGalleryGroupItemDelegate(RibbonGalleryGroup *group, QObject *parent)
        : QStyledItemDelegate(parent), m_group(group) {

}

void RibbonGalleryGroupItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const {
    if (nullptr == m_group) {
        return;
    }
    if (m_group->enableIconText()) {
        paintIconWithText(painter, option, index);
    } else {
        paintIconOnly(painter, option, index);
    }
}

void RibbonGalleryGroupItemDelegate::paintIconOnly(QPainter *painter, const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const {
    QStyle *style = m_group->style();
    painter->save();
    painter->setClipRect(option.rect);
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, m_group);
    // draw the icon
    QRect iconRect = option.rect;
    iconRect.adjust(3, 3, -3, -3);
    QIcon ico = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    ico.paint(painter, iconRect, Qt::AlignCenter, QIcon::Normal, QIcon::On);
    painter->restore();
}

void RibbonGalleryGroupItemDelegate::paintIconWithText(QPainter *painter, const QStyleOptionViewItem &option,
                                                       const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
}

QSize RibbonGalleryGroupItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    //option.rect对应grid size
    Q_UNUSED(index);
    return QSize(option.rect.width(), option.rect.height());
}




//////////////////////////////////////////

RibbonGalleryGroupModel::RibbonGalleryGroupModel(QObject *parent) : QAbstractListModel(parent) {

}

RibbonGalleryGroupModel::~RibbonGalleryGroupModel() {
    clear();
}

int RibbonGalleryGroupModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_items.size();
}

Qt::ItemFlags RibbonGalleryGroupModel::flags(const QModelIndex &index) const {
    if (!index.isValid()
        ||
        index.row() >= m_items.size())
        return Qt::NoItemFlags;
    return m_items.at(index.row())->flags();
}

QVariant RibbonGalleryGroupModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_items.count())
        return QVariant();
    return m_items.at(index.row())->data(role);
}

QModelIndex RibbonGalleryGroupModel::index(int row, int column, const QModelIndex &parent) const {
    if (hasIndex(row, column, parent))
        return createIndex(row, column, m_items.at(row));
    return QModelIndex();
}

bool RibbonGalleryGroupModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_items.count())
        return false;

    m_items.at(index.row())->setData(role, value);
    return true;
}

void RibbonGalleryGroupModel::clear() {
    beginResetModel();
    for (int i = 0; i < m_items.count(); ++i) {
        if (m_items.at(i)) {
            delete m_items.at(i);
        }
    }
    m_items.clear();
    endResetModel();
}


RibbonGalleryItem *RibbonGalleryGroupModel::at(int row) const {
    return m_items.value(row);
}

void RibbonGalleryGroupModel::insert(int row, RibbonGalleryItem *item) {
    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

RibbonGalleryItem *RibbonGalleryGroupModel::take(int row) {
    if (row < 0 || row >= m_items.count())
        return 0;

    beginRemoveRows(QModelIndex(), row, row);
    RibbonGalleryItem *item = m_items.takeAt(row);
    endRemoveRows();
    return item;
}

void RibbonGalleryGroupModel::append(RibbonGalleryItem *item) {
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count() + 1);
    m_items.append(item);
    endInsertRows();
}

//////////////////////////
/// \brief RibbonGalleryGroup::RibbonGalleryGroup
/// \param w
///////////////////////

RibbonGalleryGroup::RibbonGalleryGroup(QWidget *w) : QListView(w), m_d(new RibbonGalleryGroupPrivate(this)) {
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setSelectionRectVisible(true);
    setUniformItemSizes(true);
    setPreinstallStyle(LargeIconWithText);
    setItemDelegate(new RibbonGalleryGroupItemDelegate(this, this));

    connect(this, &QAbstractItemView::clicked, this, &RibbonGalleryGroup::onItemClicked);


}

RibbonGalleryGroup::~RibbonGalleryGroup() {
    delete m_d;
}

///
/// \brief 设置默认的预设样式
/// \param style
///
void RibbonGalleryGroup::setPreinstallStyle(RibbonGalleryGroup::PreinstallStyle style) {
    switch (style) {
        case LargeIconWithText: {
            setIconSize(QSize(72, 36));
            setGridSize(QSize(72, 56));
            setEnableIconText(true);
            break;
        }
        case LargeIconOnly: {
            setIconSize(QSize(72, 56));
            setGridSize(QSize(72, 56));
            setEnableIconText(false);
            break;
        }
        default: {
            setIconSize(QSize(72, 36));
            setGridSize(QSize(72, 56));
            setEnableIconText(true);
        }
    }
}

void RibbonGalleryGroup::addItem(const QIcon &icon) {
    if (nullptr == groupModel()) {
        return;
    }
    addItem(new RibbonGalleryItem(icon));
}

void RibbonGalleryGroup::addItem(RibbonGalleryItem *item) {
    if (nullptr == groupModel()) {
        return;
    }
    groupModel()->append(item);
}

void RibbonGalleryGroup::addActionItem(QAction *act) {
    if (nullptr == groupModel()) {
        return;
    }
    groupModel()->append(new RibbonGalleryItem(act));
}

void RibbonGalleryGroup::addActionItemList(const QList<QAction *> &acts) {
    RibbonGalleryGroupModel *model = groupModel();
    if (nullptr == model) {
        return;
    }
    for (int i = 0; i < acts.size(); ++i) {
        model->append(new RibbonGalleryItem(acts[i]));
    }
}

///
/// \brief 构建一个model，这个model的父类是RibbonGalleryGroup，如果要共享model，需要手动处理model的父类
///
void RibbonGalleryGroup::setupGroupModel() {
    setModel(new RibbonGalleryGroupModel(this));
}

RibbonGalleryGroupModel *RibbonGalleryGroup::groupModel() {
    return qobject_cast<RibbonGalleryGroupModel *>(model());
}

void RibbonGalleryGroup::setEnableIconText(bool enable) {
    m_d->enableIconText = enable;
}

bool RibbonGalleryGroup::enableIconText() const {
    return m_d->enableIconText;
}

void RibbonGalleryGroup::setGroupTitle(const QString &title) {
    m_d->groupTitle = title;
    emit groupTitleChanged(m_d->groupTitle);
}

QString RibbonGalleryGroup::groupTitle() const {
    return m_d->groupTitle;
}

void RibbonGalleryGroup::onItemClicked(const QModelIndex &index) {
    if (index.isValid()) {
        RibbonGalleryItem *item = (RibbonGalleryItem *) index.internalPointer();
        if (item) {
            QAction *act = item->action();
            if (act) {
                act->activate(QAction::Trigger);
            }
        }
    }
}