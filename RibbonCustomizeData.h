//
// Created by weiwu on 21-5-2.
//

#ifndef RIBBON_RIBBONCUSTOMIZEDATA_H
#define RIBBON_RIBBONCUSTOMIZEDATA_H


#include <QtCore/QMetaType>
#include <QtCore/QString>
#include "RibbonActionsManager.h"

class RibbonCustomizeData {
public:
    enum ActionType {
        UnknowActionType = 0            ///< 未知操作
        , AddCategoryActionType         ///< 添加category操作(1)
        , AddPannelActionType           ///< 添加pannel操作(2)
        , AddActionActionType           ///< 添加action操作(3)
        , RemoveCategoryActionType      ///< 删除category操作(4)
        , RemovePannelActionType        ///< 删除pannel操作(5)
        , RemoveActionActionType        ///< 删除action操作(6)
        , ChangeCategoryOrderActionType ///< 改变category顺序的操作(7)
        , ChangePannelOrderActionType   ///< 改变pannel顺序的操作(8)
        , ChangeActionOrderActionType   ///< 改变action顺序的操作(9)
        , RenameCategoryActionType      ///< 对category更名操作(10)
        , RenamePannelActionType        ///< 对Pannel更名操作(11)
        , VisibleCategoryActionType     ///< 对category执行隐藏/显示操作(12)
    };
    RibbonCustomizeData();
    RibbonCustomizeData(ActionType type, RibbonActionsManager *mgr = nullptr);
    //获取CustomizeData的action type
    ActionType actionType() const;

    //设置CustomizeData的action type
    void setActionType(ActionType a);

    //判断是否是一个正常的CustomizeData
    bool isValid() const;

    //应用RibbonCustomizeData
    bool apply(RibbonMainWindow *m) const;

    //获取actionmanager指针
    RibbonActionsManager *actionManager();

    //设置ActionsManager
    void setActionsManager(RibbonActionsManager *mgr);

    //对应AddCategoryActionType
    static RibbonCustomizeData makeAddCategoryCustomizeData(const QString& title
            , int index
            , const QString& objName);

    //对应AddPannelActionType
    static RibbonCustomizeData makeAddPannelCustomizeData(const QString& title
            , int index
            , const QString& categoryobjName
            , const QString& objName);

    //对应AddActionActionType
    static RibbonCustomizeData makeAddActionCustomizeData(const QString& key
            , RibbonActionsManager *mgr
            , RibbonPannelItem::RowProportion rp
            , const QString& categoryObjName
            , const QString& pannelObjName
    );

    //对应RenameCategoryActionType
    static RibbonCustomizeData makeRenameCategoryCustomizeData(const QString& newname
            , const QString& categoryobjName);

    //对应RenamePannelActionType
    static RibbonCustomizeData makeRenamePannelCustomizeData(const QString& newname
            , const QString& categoryobjName
            , const QString& pannelObjName);

    //对应RemoveCategoryActionType
    static RibbonCustomizeData makeRemoveCategoryCustomizeData(const QString& categoryobjName);

    //对应ChangeCategoryOrderActionType
    static RibbonCustomizeData makeChangeCategoryOrderCustomizeData(const QString& categoryobjName
            , int moveindex);

    //对应ChangePannelOrderActionType
    static RibbonCustomizeData makeChangePannelOrderCustomizeData(const QString& categoryobjName
            , const QString& pannelObjName
            , int moveindex);

    //对应ChangeActionOrderActionType
    static RibbonCustomizeData makeChangeActionOrderCustomizeData(const QString& categoryobjName
            , const QString& pannelObjName
            , const QString& key
            , RibbonActionsManager *mgr
            , int moveindex);

    //对应RemovePannelActionType
    static RibbonCustomizeData makeRemovePannelCustomizeData(const QString& categoryobjName
            , const QString& pannelObjName);

    //对应RemoveActionActionType
    static RibbonCustomizeData makeRemoveActionCustomizeData(const QString& categoryobjName
            , const QString& pannelObjName
            , const QString& key
            , RibbonActionsManager *mgr);

    //对应VisibleCategoryActionType
    static RibbonCustomizeData makeVisibleCategoryCustomizeData(const QString& categoryobjName, bool isShow);

    //判断是否可以自定义,如果某个action不想被编辑，可以通过此函数设置
    static bool isCanCustomize(QObject *obj);
    static void setCanCustomize(QObject *obj, bool canbe = true);

    //对QList<RibbonCustomizeData>进行简化
    static QList<RibbonCustomizeData> simplify(const QList<RibbonCustomizeData>& csd);

public:

    /**
     * @brief 记录顺序的参数
     *
     * 在actionType==AddCategoryActionType时，此参数记录Category的insert位置,
     * 在actionType==AddPannelActionType时，此参数记录pannel的insert位置,
     * 在actionType==AddActionActionType时，此参数记录pannel的insert位置
     */
    int indexValue;

    /**
     * @brief 记录标题、索引等参数
     *
     * 在actionType==AddCategoryActionType时，key为category标题，
     * 在actionType==AddPannelActionType时，key为pannel标题，
     * 在actionType==AddActionActionType时，key为action的查询依据，基于RibbonActionsManager::action查询
     */
    QString keyValue;

    /**
     * @brief 记录categoryObjName，用于定位Category
     */
    QString categoryObjNameValue;

    /**
     * @brief 记录pannelObjName，saribbon的Customize索引大部分基于objname
     */
    QString pannelObjNameValue;

    RibbonPannelItem::RowProportion actionRowProportionValue;     ///< 行的占比，ribbon中有large，media和small三种占比,见@ref RowProportion
private:
    ActionType m_type;                                              ///< 标记这个data是category还是pannel亦或是action
    RibbonActionsManager *m_actionsManagerPointer;
};
Q_DECLARE_METATYPE(RibbonCustomizeData)

typedef QList<RibbonCustomizeData> RibbonCustomizeDataList;
#endif //RIBBON_RIBBONCUSTOMIZEDATA_H
