//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONGALLERYGROUP_H
#define RIBBON_RIBBONGALLERYGROUP_H

#include <QList>
#include <QListView>
#include <QStyledItemDelegate>
#include <RibbonGalleryItem.h>

/**
 * RibbonGalleryGroup 对应的显示代理
 */
class RibbonGalleryGroupItemDelegate : public QStyledItemDelegate {
public:
    RibbonGalleryGroupItemDelegate(RibbonGalleryGroup *group, QObject *parent = Q_NULLPTR);

    virtual void
    paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual void paintIconOnly(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual void
    paintIconWithText(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    RibbonGalleryGroup *m_group;
};
/**
 * RibbonGalleryGroup 对应的model
 */
class RibbonGalleryGroupModel : public QAbstractListModel {
Q_OBJECT
public:
    RibbonGalleryGroupModel(QObject *parent = Q_NULLPTR);

    ~RibbonGalleryGroupModel();

    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    void clear();

    RibbonGalleryItem *at(int row) const;

    void insert(int row, RibbonGalleryItem *item);

    RibbonGalleryItem *take(int row);

    void append(RibbonGalleryItem *item);

private:
    QList<RibbonGalleryItem *> m_items;
};

class RibbonGalleryGroupPrivate;

/**
 * 组负责显示管理Gallery Item
 */

class RibbonGalleryGroup : public QListView {
Q_OBJECT
    Q_PROPERTY(bool enableIconText
                       READ
                       enableIconText
                       WRITE
                       setEnableIconText)
public:
    ///
    /// \brief 预设样式
    ///
    enum PreinstallStyle {
        LargeIconWithText ///< 大图标带文字
        , LargeIconOnly

    };

    RibbonGalleryGroup(QWidget *w = 0);

    virtual ~RibbonGalleryGroup();

    void setPreinstallStyle(PreinstallStyle style);

    void addItem(const QIcon &icon);

    void addItem(RibbonGalleryItem *item);

    void addActionItem(QAction *act);

    void addActionItemList(const QList<QAction *> &acts);

    //构建一个model，这个model的父类是SARibbonGalleryGroup，如果要共享model，需要手动处理model的父类
    void setupGroupModel();

    RibbonGalleryGroupModel *groupModel();

    //是否在Gallery的图标下显示文字
    void setEnableIconText(bool enable);

    bool enableIconText() const;

    void setGroupTitle(const QString &title);

    QString groupTitle() const;

private slots:

    void onItemClicked(const QModelIndex &index);

signals:

    void groupTitleChanged(const QString &title);

private:
    RibbonGalleryGroupPrivate *m_d;
};


#endif //RIBBON_RIBBONGALLERYGROUP_H
