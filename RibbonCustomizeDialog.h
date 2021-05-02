//
// Created by weiwu on 21-5-2.
//

#ifndef RIBBON_RIBBONCUSTOMIZEDIALOG_H
#define RIBBON_RIBBONCUSTOMIZEDIALOG_H

#include <QDialog>
#include "RibbonActionsManager.h"
#include "RibbonCustomizeWidget.h"

class RibbonCustomizeDialogUi;

class RibbonCustomizeDialog : public QDialog{
    Q_OBJECT
public:
    RibbonCustomizeDialog(RibbonMainWindow *ribbonWindow, QWidget *p = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    //设置action管理器
    void setupActionsManager(RibbonActionsManager *mgr);

    //应用所有的设定
    bool applys();

    //清除所有动作
    void clear();

    //转换为xml
    bool toXml(QXmlStreamWriter *xml) const;
    bool toXml(const QString& xmlpath) const;

    //从xml中加载QList<RibbonCustomizeData>，对于基于配置文件的设置，对话框显示前建议调用此函数，保证叠加设置的正确记录
    void fromXml(QXmlStreamReader *xml);
    void fromXml(const QString& xmlpath);

    //返回RibbonCustomizeWidget窗口指针
    RibbonCustomizeWidget *customizeWidget() const;

private:
    void initConnection();

    RibbonCustomizeDialogUi *ui;
};


#endif //RIBBON_RIBBONCUSTOMIZEDIALOG_H
