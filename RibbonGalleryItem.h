//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONGALLERYITEM_H
#define RIBBON_RIBBONGALLERYITEM_H

#include <QIcon>
#include <QVariant>
#include <QMap>
#include <QAction>
class RibbonGalleryGroup;

/**
 * 类似QStandardItem的GalleryItem
 */
class RibbonGalleryItem {
public:
    RibbonGalleryItem();
    RibbonGalleryItem(const QIcon& icon);
    RibbonGalleryItem(QAction* action);
    virtual ~RibbonGalleryItem();

    void setData(int role, const QVariant& data);
    virtual QVariant data(int role) const;

    void setText(const QString& text);
    QString text() const;

    void setToolTip(const QString& text);
    QString toolTip() const;

    void setIcon(const QIcon& icon);
    QIcon icon() const;


    bool isSelectable() const;
    void setSelectable(bool isSelectable);

    bool isEnable() const;
    void setEnable(bool isEnable);

    void setFlags(Qt::ItemFlags flag);
    virtual Qt::ItemFlags flags() const;

    void setAction(QAction* action);
    QAction* action();
private:
    friend class SARibbonGalleryGroupModel;
    QMap<int,QVariant> m_datas;
    Qt::ItemFlags m_flsgs;
    QAction* m_action;

};


#endif //RIBBON_RIBBONGALLERYITEM_H
