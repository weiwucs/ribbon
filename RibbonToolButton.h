//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONTOOLBUTTON_H
#define RIBBON_RIBBONTOOLBUTTON_H


#include <QtWidgets/QToolButton>

class RibbonToolButton : public QToolButton {
    Q_OBJECT
    Q_ENUMS(RibbonButtonType)

public:
    enum RibbonButtonType {
        LargeButton, SmallButton
    };

    enum LargeButtonType{
        Normal, Lite
    };

    RibbonToolButton(QWidget *parent = Q_NULLPTR);
    RibbonToolButton(QAction *defaultAction, QWidget *parent = Q_NULLPTR);
    RibbonButtonType buttonType() const;
    void setButtonType(const RibbonButtonType &buttonType);
    virtual QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    void setLargeButtonType(LargeButtonType type);
    LargeButtonType largeButtonType() const;

    //lite模式下的分割线
    virtual int liteLargeButtonIconHeight(int buttonHeight) const;

protected:
    void calcIconRect(const QStyleOptionToolButton &opt);
    QRect calcTextRect(const QStyleOptionToolButton &opt) const;
    QRect calcTextRect(const QRect &buttonRect, bool hasMenu = false) const;
    QRect calcIndicatorArrowDownRect(const QStyleOptionToolButton &opt);
    QPixmap createIconPixmap(const QStyleOptionToolButton &opt);
    bool event(QEvent *e) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    virtual void focusOutEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    virtual void paintLargeButton(QPaintEvent *e);
    virtual void paintSmallButton(QPaintEvent *e);
    virtual bool hitButton(const QPoint &pos) const Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;
    virtual void drawIconAndLabel(QPainter &p, QStyleOptionToolButton &opt);

private:
    static void drawArrow(const QStyle *style, const QStyleOptionToolButton *toolbutton,
                          const QRect &rect, QPainter *painter, const QWidget *widget = 0);

private:
    RibbonButtonType m_buttonType;
    LargeButtonType m_largeButtonType;
    bool m_mouseOnSubControl; ///< 这个用于标记MenuButtonPopup模式下，鼠标在文本区域
    bool m_menuButtonPressed;
    QRect m_iconRect;
    bool m_isWordWrap;///< 标记是否文字换行 @default false
};


#endif //RIBBON_RIBBONTOOLBUTTON_H
