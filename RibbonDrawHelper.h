//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONDRAWHELPER_H
#define RIBBON_RIBBONDRAWHELPER_H


#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStylePainter>

class RibbonDrawHelper {
public:
    RibbonDrawHelper();

    static QPixmap iconToPixmap(const QIcon &icon, QWidget *widget, const QStyleOption *opt, const QSize &icoSize);

    static void drawIcon(const QIcon &icon, QPainter *painter, const QStyleOption *opt, int x, int y, int width, int height);

    static void drawIcon(const QIcon &icon, QPainter *painter, const QStyleOption *opt, const QRect &rect);

    static QSize iconActualSize(const QIcon &icon, const QStyleOption *opt, const QSize &iconSize);

    static void drawText(const QString &text, QStylePainter *painter, const QStyleOption *opt, Qt::Alignment al, int x, int y,
             int width, int height);

    static void drawText(const QString &text, QStylePainter *painter, const QStyleOption *opt, Qt::Alignment al, const QRect &rect);
};


#endif //RIBBON_RIBBONDRAWHELPER_H
