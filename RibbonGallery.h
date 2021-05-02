//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONGALLERY_H
#define RIBBON_RIBBONGALLERY_H

#include <QtWidgets/QFrame>
#include "RibbonGalleryGroup.h"

class QVBoxLayout;

class RibbonGalleryPrivate;

class RibbonGalleryViewport;

class RibbonGalleryViewport : public QWidget {
Q_OBJECT
public:
    RibbonGalleryViewport(QWidget *parent);

    void addWidget(QWidget *w);

private:
    QVBoxLayout *m_layout;
};


class RibbonGallery : public QFrame {
Q_OBJECT
public:
    RibbonGallery(QWidget *parent = 0);

    virtual ~RibbonGallery();

    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

    virtual QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    RibbonGalleryGroup *addGalleryGroup();

    RibbonGalleryGroup *addCategoryActions(const QString &title, QList<QAction *> actions);

    void setCurrentViewGroup(RibbonGalleryGroup *group);

    RibbonGalleryGroup *currentViewGroup() const;

protected slots:

    virtual void onPageDown();

    virtual void onPageUp();

    virtual void onShowMoreDetail();

    void onItemClicked(const QModelIndex &index);

private:
    RibbonGalleryViewport *ensureGetPopupViewPort();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    friend class RibbonGalleryPrivate;

    RibbonGalleryPrivate *m_d;
};


#endif //RIBBON_RIBBONGALLERY_H
