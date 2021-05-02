//
// Created by weiwu on 21-5-2.
//

#ifndef RIBBON_RIBBONCUSTOMIZEWIDGET_H
#define RIBBON_RIBBONCUSTOMIZEWIDGET_H


#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include <QtCore/QXmlStreamReader>
#include <QtWidgets/QAbstractButton>
#include "RibbonActionsManager.h"
#include "RibbonCustomizeData.h"

class RibbonCustomizeWidgetUi;
class RibbonCustomizeWidgetPrivate;

class RibbonCustomizeWidget : public QWidget{
Q_OBJECT
public:
    RibbonCustomizeWidget(RibbonMainWindow *ribbonWindow, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~RibbonCustomizeWidget();

    /**
     * @brief 定义ribbon树的显示类型
     */
    enum RibbonTreeShowType {
        ShowAllCategory         ///< 显示所有Category，包括contextcategory
        , ShowMainCategory      ///< 显示主要的category，不包含上下文
    };

    /**
     * @brief QStandardItem对应的role
     */
    enum ItemRole {
        LevelRole		= Qt::UserRole + 1      ///< 代表这是层级，有0：category 1：pannel 2：item
        , PointerRole		= Qt::UserRole + 2      ///< 代表这是存放指针。根据LevelRole来进行转
        , CanCustomizeRole	= Qt::UserRole + 3      ///< 代表个item是可以自定义的.bool
        , CustomizeRole		= Qt::UserRole + 4      ///< 代表这个是自定义的item,bool,主要用于那些自己添加的标签和pannel，有此角色必有CanCustomizeRole
        , CustomizeObjNameRole	= Qt::UserRole + 5      ///< 记录了临时的自定义内容的obj名 QString
    };



    //设置action管理器
    void setupActionsManager(RibbonActionsManager *mgr);

    //判断用户是否有改动内容
    bool isChanged() const;

    //获取model
    const QStandardItemModel *model() const;

    //根据当前的radiobutton选项来更新model
    void updateModel();

    //更新model
    void updateModel(RibbonTreeShowType type);

    //应用所有的设定
    bool applys();

    //转换为xml
    bool toXml(QXmlStreamWriter *xml) const;
    bool toXml(const QString& xmlpath) const;

    //从xml中加载QList<PrivateRibbonCustomizeData>，对于基于配置文件的设置，对话框显示前建议调用此函数，保证叠加设置的正确记录
    void fromXml(QXmlStreamReader *xml);
    void fromXml(const QString& xmlpath);

    //应用xml配置，可以结合customize_datas_from_xml和customize_datas_apply函数
    static bool fromXml(QXmlStreamReader *xml, RibbonMainWindow *w, RibbonActionsManager *mgr);

    //清除所有动作，在执行applys函数后，如果要继续调用，应该clear，否则会导致异常
    void clear();


protected:
    //把QList<PrivateRibbonCustomizeData>进行裁剪,把一些动作合并
    void simplify();

    RibbonPannelItem::RowProportion selectedRowProportion() const;

    QAction *selectedAction() const;
    QAction *itemToAction(QStandardItem *item) const;

    QStandardItem *selectedItem() const;


    //获取选中的ribbon tree 的level
    int selectedRibbonLevel() const;

    //根据选中的item判断
    int itemLevel(QStandardItem *item) const;

    //设置某个item被选中
    void setSelectItem(QStandardItem *item, bool ensureVisible = true);

    //判断itemn能否改动，可以改动返回true
    bool isItemCanCustomize(QStandardItem *item) const;
    bool isSelectedItemCanCustomize() const;

    //判断item是否是自定义的item
    bool isCustomizeItem(QStandardItem *item) const;
    bool isSelectedItemIsCustomize() const;

    //删除一个item
    void removeItem(QStandardItem *item);

private slots:
    void onComboBoxActionIndexCurrentIndexChanged(int index);
    void onRadioButtonGroupButtonClicked(QAbstractButton *b);
    void onPushButtonNewCategoryClicked();
    void onPushButtonNewPannelClicked();
    void onPushButtonRenameClicked();
    void onPushButtonAddClicked();
    void onPushButtonDeleteClicked();
    void onListViewSelectClicked(const QModelIndex& index);
    void onTreeViewResultClicked(const QModelIndex& index);
    void onToolButtonUpClicked();
    void onToolButtonDownClicked();
    void onItemChanged(QStandardItem *item);
    void onLineEditSearchActionTextEdited(const QString& text);

private:
    void initConnection();

private:
    RibbonCustomizeWidgetUi *ui;
    RibbonCustomizeWidgetPrivate *m_d;
};

bool  sa_customize_datas_to_xml(QXmlStreamWriter *xml, const QList<RibbonCustomizeData>& cds);

QList<RibbonCustomizeData>  sa_customize_datas_from_xml(QXmlStreamReader *xml, RibbonActionsManager *mgr);

int  sa_customize_datas_apply(const QList<RibbonCustomizeData>& cds, RibbonMainWindow *w);

bool  sa_apply_customize_from_xml_file(const QString& filePath, RibbonMainWindow *w, RibbonActionsManager *mgr);

#endif //RIBBON_RIBBONCUSTOMIZEWIDGET_H
