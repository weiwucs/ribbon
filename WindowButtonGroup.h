//
// Created by weiwu on 21-5-1.
//

#ifndef RIBBON_WINDOWBUTTONGROUP_H
#define RIBBON_WINDOWBUTTONGROUP_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>

class WindowButtonGroupPrivate;

class WindowButtonGroup : public QWidget {
Q_OBJECT
public:
    WindowButtonGroup(QWidget *parent);

    WindowButtonGroup(QWidget *parent, Qt::WindowFlags flags);

    ~WindowButtonGroup();

    void setupMinimizeButton(bool on);

    void setupMaximizeButton(bool on);

    void setupCloseButton(bool on);

    void updateWindowFlag();

    //设置按钮的宽度比例,最终按钮宽度将按照此比例进行设置
    void setButtonWidthStretch(int close = 4, int max = 3, int min = 3);

    //设置按钮的缩放比例
    void setIconScale(qreal iconscale = 0.5);

    //设置Qt::WindowStates
    void setWindowStates(Qt::WindowStates s);

protected:
    virtual QSize sizeHint() const;

    virtual bool eventFilter(QObject *watched, QEvent *e);

    virtual void parentResize();

    virtual void resizeEvent(QResizeEvent *e);

private:
    //void updateMaximizeButtonIcon();

protected slots:
    Q_SLOT void closeWindow();

    Q_SLOT void minimizeWindow();

    Q_SLOT void maximizeWindow();

private:
    friend class WindowButtonGroupPrivate;

    WindowButtonGroupPrivate *m_d;
};

class WindowToolButton : public QToolButton {
Q_OBJECT
public:
    WindowToolButton(QWidget *p = nullptr);
};


#endif //RIBBON_WINDOWBUTTONGROUP_H
