//
// Created by weiwu on 21-5-2.
//

#include "RibbonCustomizeData.h"

#ifndef SA_RIBBON_BAR_PROP_CAN_CUSTOMIZE
#define SA_RIBBON_BAR_PROP_CAN_CUSTOMIZE    "_sa_isCanCustomize"
#endif

RibbonCustomizeData::RibbonCustomizeData()
        : indexValue(-1), actionRowProportionValue(RibbonPannelItem::Large), m_type(UnknowActionType),
          m_actionsManagerPointer(nullptr) {
}


RibbonCustomizeData::RibbonCustomizeData(ActionType type, RibbonActionsManager *mgr)
        : indexValue(-1), actionRowProportionValue(RibbonPannelItem::Large), m_type(type),
          m_actionsManagerPointer(mgr) {
}


/**
 * @brief 获取CustomizeData的action type
 * @return
 */
RibbonCustomizeData::ActionType RibbonCustomizeData::actionType() const {
    return (m_type);
}


/**
 * @brief 设置CustomizeData的action type
 * @param a
 */
void RibbonCustomizeData::setActionType(RibbonCustomizeData::ActionType a) {
    m_type = a;
}


/**
 * @brief 判断是否是一个正常的CustomizeData
 *
 * 实际逻辑actionType() != UnknowActionType
 * @return 有用的CustomizeData返回true
 */
bool RibbonCustomizeData::isValid() const {
    return (actionType() != UnknowActionType);
}


/**
 * @brief 应用RibbonCustomizeData到RibbonMainWindow
 * @param m
 * @return 如果应用失败，返回false,如果actionType==UnknowActionType直接返回false
 */
bool RibbonCustomizeData::apply(RibbonMainWindow *m) const {
    RibbonBar *bar = m->ribbonBar();

    if (nullptr == bar) {
        return (false);
    }
    switch (actionType()) {
        case UnknowActionType:
            return (false);

        case AddCategoryActionType: {
            //添加标签
            RibbonCategory *c = bar->insertCategoryPage(keyValue, indexValue);
            if (nullptr == c) {
                return (false);
            }
            c->setObjectName(categoryObjNameValue);
            RibbonCustomizeData::setCanCustomize(c);
            return (true);
        }

        case AddPannelActionType: {
            //添加pannel
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            RibbonPannel *p = c->insertPannel(keyValue, indexValue);
            p->setObjectName(pannelObjNameValue);
            RibbonCustomizeData::setCanCustomize(p);
            return (true);
        }

        case AddActionActionType: {
            if (nullptr == m_actionsManagerPointer) {
                return (false);
            }
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            RibbonPannel *pannel = c->pannelByObjectName(pannelObjNameValue);
            if (nullptr == pannel) {
                return (false);
            }
            QAction *act = m_actionsManagerPointer->action(keyValue);
            if (nullptr == act) {
                return (false);
            }
            pannel->addAction(act, actionRowProportionValue);
            return (true);
        }

        case RemoveCategoryActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            bar->removeCategory(c);
            return (true);
        }

        case RemovePannelActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            RibbonPannel *pannel = c->pannelByObjectName(pannelObjNameValue);
            if (nullptr == pannel) {
                return (false);
            }
            c->removePannel(pannel);
            return (true);
        }

        case RemoveActionActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            RibbonPannel *pannel = c->pannelByObjectName(pannelObjNameValue);
            if (nullptr == pannel) {
                return (false);
            }
            QAction *act = m_actionsManagerPointer->action(keyValue);
            if (nullptr == act) {
                return (false);
            }
            pannel->removeAction(act);
            return (true);
        }

        case ChangeCategoryOrderActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            int currentindex = bar->categoryIndex(c);
            if (-1 == currentindex) {
                return (false);
            }
            int toindex = currentindex + indexValue;
            bar->moveCategory(currentindex, toindex);
            return (true);
        }

        case ChangePannelOrderActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            RibbonPannel *pannel = c->pannelByObjectName(pannelObjNameValue);
            if (nullptr == pannel) {
                return (false);
            }
            int pannelIndex = c->pannelIndex(pannel);
            if (-1 == pannelIndex) {
                return (false);
            }
            c->movePannel(pannelIndex, pannelIndex + indexValue);
            return (true);
        }

        case ChangeActionOrderActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            RibbonPannel *pannel = c->pannelByObjectName(pannelObjNameValue);
            if (nullptr == pannel) {
                return (false);
            }
            QAction *act = m_actionsManagerPointer->action(keyValue);
            if (nullptr == act) {
                return (false);
            }
            int actindex = pannel->actionIndex(act);
            if (actindex <= -1) {
                return (false);
            }
            pannel->moveAction(actindex, actindex + indexValue);
            return (true);
        }

        case RenameCategoryActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            c->setWindowTitle(keyValue);
            return (true);
        }

        case RenamePannelActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            RibbonPannel *pannel = c->pannelByObjectName(pannelObjNameValue);
            if (nullptr == pannel) {
                return (false);
            }
            pannel->setWindowTitle(keyValue);
            return (true);
        }

        case VisibleCategoryActionType: {
            RibbonCategory *c = bar->categoryByObjectName(categoryObjNameValue);
            if (nullptr == c) {
                return (false);
            }
            if (1 == indexValue) {
                bar->showCategory(c);
            } else {
                bar->hideCategory(c);
            }
            return (true);
        }

        default:
            break;
    }
    return (false);
}


/**
 * @brief 获取actionmanager指针
 * @return
 */
RibbonActionsManager *RibbonCustomizeData::actionManager() {
    return (m_actionsManagerPointer);
}


/**
 * @brief 设置ActionsManager
 * @param mgr
 */
void RibbonCustomizeData::setActionsManager(RibbonActionsManager *mgr) {
    m_actionsManagerPointer = mgr;
}


/**
 * @brief 创建一个AddCategoryActionType的RibbonCustomizeData
 * @param title category 的标题
 * @param index category要插入的位置
 * @param objName category的object name
 * @return 返回AddCategoryActionType的RibbonCustomizeData
 */
RibbonCustomizeData
RibbonCustomizeData::makeAddCategoryCustomizeData(const QString &title, int index, const QString &objName) {
    RibbonCustomizeData d(AddCategoryActionType);

    d.indexValue = index;
    d.keyValue = title;
    d.categoryObjNameValue = objName;
    return (d);
}


/**
 * @brief 创建一个AddPannelActionType的RibbonCustomizeData
 * @param title pannel的标题
 * @param index pannel的index
 * @param categoryobjName pannel的category的objectname
 * @param objName pannel的objname
 * @return 返回AddPannelActionType的RibbonCustomizeData
 */
RibbonCustomizeData
RibbonCustomizeData::makeAddPannelCustomizeData(const QString &title, int index, const QString &categoryobjName,
                                                  const QString &objName) {
    RibbonCustomizeData d(AddPannelActionType);

    d.indexValue = index;
    d.keyValue = title;
    d.pannelObjNameValue = objName;
    d.categoryObjNameValue = categoryobjName;
    return (d);
}


/**
 * @brief 添加action
 * @param key action的索引
 * @param mgr action管理器
 * @param rp 定义action的占位情况
 * @param categoryObjName action添加到的category的objname
 * @param pannelObjName action添加到的category下的pannel的objname
 * @param index action添加到的pannel的索引
 * @return
 */
RibbonCustomizeData RibbonCustomizeData::makeAddActionCustomizeData(const QString &key, RibbonActionsManager *mgr,
                                                                        RibbonPannelItem::RowProportion rp,
                                                                        const QString &categoryObjName,
                                                                        const QString &pannelObjName) {
    RibbonCustomizeData d(AddActionActionType, mgr);

    d.keyValue = key;
    d.categoryObjNameValue = categoryObjName;
    d.pannelObjNameValue = pannelObjName;
    d.actionRowProportionValue = rp;

    return (d);
}


/**
 * @brief 创建一个RenameCategoryActionType的RibbonCustomizeData
 * @param newname 新名字
 * @param index category的索引
 * @return 返回RenameCategoryActionType的RibbonCustomizeData
 */
RibbonCustomizeData
RibbonCustomizeData::makeRenameCategoryCustomizeData(const QString &newname, const QString &categoryobjName) {
    RibbonCustomizeData d(RenameCategoryActionType);

    d.keyValue = newname;
    d.categoryObjNameValue = categoryobjName;
    return (d);
}


/**
 * @brief 创建一个RenamePannelActionType的RibbonCustomizeData
 * @param newname pannel的名字
 * @param indexValue pannel的索引
 * @param categoryobjName pannel对应的category的object name
 * @return 返回RenamePannelActionType的RibbonCustomizeData
 */
RibbonCustomizeData
RibbonCustomizeData::makeRenamePannelCustomizeData(const QString &newname, const QString &categoryobjName,
                                                     const QString &pannelObjName) {
    RibbonCustomizeData d(RenamePannelActionType);

    d.keyValue = newname;
    d.pannelObjNameValue = pannelObjName;
    d.categoryObjNameValue = categoryobjName;
    return (d);
}


/**
 * @brief 对应RemoveCategoryActionType
 * @param categoryobjName 需要移除的objname
 * @return
 */
RibbonCustomizeData RibbonCustomizeData::makeRemoveCategoryCustomizeData(const QString &categoryobjName) {
    RibbonCustomizeData d(RemoveCategoryActionType);

    d.categoryObjNameValue = categoryobjName;
    return (d);
}


/**
 * @brief 对应ChangeCategoryOrderActionType
 * @param categoryobjName 需要移动的categoryobjName
 * @param moveindex 移动位置，-1代表向上（向左）移动一个位置，1带表向下（向右）移动一个位置
 * @return
 */
RibbonCustomizeData
RibbonCustomizeData::makeChangeCategoryOrderCustomizeData(const QString &categoryobjName, int moveindex) {
    RibbonCustomizeData d(ChangeCategoryOrderActionType);

    d.categoryObjNameValue = categoryobjName;
    d.indexValue = moveindex;
    return (d);
}


/**
 * @brief 对应ChangePannelOrderActionType
 * @param categoryobjName 需要移动的pannel对应的categoryobjName
 * @param pannelObjName 需要移动的pannelObjName
 * @param moveindex 移动位置，-1代表向上（向左）移动一个位置，1带表向下（向右）移动一个位置
 * @return
 */
RibbonCustomizeData
RibbonCustomizeData::makeChangePannelOrderCustomizeData(const QString &categoryobjName, const QString &pannelObjName,
                                                          int moveindex) {
    RibbonCustomizeData d(ChangePannelOrderActionType);

    d.categoryObjNameValue = categoryobjName;
    d.pannelObjNameValue = pannelObjName;
    d.indexValue = moveindex;
    return (d);
}


/**
 * @brief 对应ChangeActionOrderActionType
 * @param categoryobjName 需要移动的pannel对应的categoryobjName
 * @param pannelObjName 需要移动的pannelObjName
 * @param key RibbonActionsManager管理的key名
 * @param mgr RibbonActionsManager指针
 * @param moveindex 移动位置，-1代表向上（向左）移动一个位置，1带表向下（向右）移动一个位置
 * @return
 */
RibbonCustomizeData
RibbonCustomizeData::makeChangeActionOrderCustomizeData(const QString &categoryobjName, const QString &pannelObjName,
                                                          const QString &key, RibbonActionsManager *mgr,
                                                          int moveindex) {
    RibbonCustomizeData d(ChangeActionOrderActionType, mgr);

    d.categoryObjNameValue = categoryobjName;
    d.pannelObjNameValue = pannelObjName;
    d.keyValue = key;
    d.indexValue = moveindex;
    return (d);
}


/**
 * @brief 对应RemovePannelActionType
 * @param categoryobjName pannel对应的category的obj name
 * @param pannelObjName pannel对应的 obj name
 * @return
 */
RibbonCustomizeData
RibbonCustomizeData::makeRemovePannelCustomizeData(const QString &categoryobjName, const QString &pannelObjName) {
    RibbonCustomizeData d(RemovePannelActionType);

    d.categoryObjNameValue = categoryobjName;
    d.pannelObjNameValue = pannelObjName;
    return (d);
}


/**
 * @brief 对应RemoveActionActionType
 * @param categoryobjName pannel对应的category的obj name
 * @param pannelObjName pannel对应的 obj name
 * @param key RibbonActionsManager管理的key名
 * @param mgr RibbonActionsManager指针
 * @return
 */
RibbonCustomizeData
RibbonCustomizeData::makeRemoveActionCustomizeData(const QString &categoryobjName, const QString &pannelObjName,
                                                     const QString &key, RibbonActionsManager *mgr) {
    RibbonCustomizeData d(RemoveActionActionType, mgr);

    d.categoryObjNameValue = categoryobjName;
    d.pannelObjNameValue = pannelObjName;
    d.keyValue = key;
    return (d);
}


/**
 * @brief RibbonCustomizeData::makeVisibleCategoryCustomizeData
 * @param categoryobjName
 * @param isShow
 * @return
 */
RibbonCustomizeData
RibbonCustomizeData::makeVisibleCategoryCustomizeData(const QString &categoryobjName, bool isShow) {
    RibbonCustomizeData d(VisibleCategoryActionType);

    d.categoryObjNameValue = categoryobjName;
    d.indexValue = isShow ? 1 : 0;
    return (d);
}


/**
 * @brief 判断外置属性，是否允许自定义
 * @param obj
 * @return
 */
bool RibbonCustomizeData::isCanCustomize(QObject *obj) {
    QVariant v = obj->property(SA_RIBBON_BAR_PROP_CAN_CUSTOMIZE);

    if (v.isValid()) {
        return (v.toBool());
    }
    return (false);
}


/**
 * @brief 设置外置属性允许自定义
 * @param obj
 * @param canbe
 */
void RibbonCustomizeData::setCanCustomize(QObject *obj, bool canbe) {
    obj->setProperty(SA_RIBBON_BAR_PROP_CAN_CUSTOMIZE, canbe);
}


QList<RibbonCustomizeData> remove_indexs(const QList<RibbonCustomizeData> &csd, const QList<int> &willremoveIndex);

QList<RibbonCustomizeData> remove_indexs(const QList<RibbonCustomizeData> &csd, const QList<int> &willremoveIndex) {
    QList<RibbonCustomizeData> res;

    for (int i = 0; i < csd.size(); ++i) {
        if (!willremoveIndex.contains(i)) {
            res << csd[i];
        }
    }
    return (res);
}


/**
 * @brief 对QList<RibbonCustomizeData>进行简化操作
 *
 * 此函数会执行如下操作：
 * 1、针对同一个category/pannel连续出现的添加和删除操作进行移除（前一步添加，后一步删除）
 *
 * 2、针对VisibleCategoryActionType，对于连续出现的操作只保留最后一步
 *
 * 3、针对RenameCategoryActionType和RenamePannelActionType操作，只保留最后一个
 *
 * 4、针对连续的ChangeCategoryOrderActionType，ChangePannelOrderActionType，ChangeActionOrderActionType进行合并为一个动作，
 * 如果合并后原地不动，则删除
 *
 * @param csd
 * @return 返回简化的QList<RibbonCustomizeData>
 */
QList<RibbonCustomizeData> RibbonCustomizeData::simplify(const QList<RibbonCustomizeData> &csd) {
    int size = csd.size();

    if (size <= 1) {
        return (csd);
    }
    QList<RibbonCustomizeData> res;
    QList<int> willremoveIndex;//记录要删除的index

    //! 首先针对连续出现的添加和删除操作进行优化
    for (int i = 1; i < size; ++i) {
        if ((csd[i - 1].actionType() == AddCategoryActionType) &&
            (csd[i].actionType() == RemoveCategoryActionType)) {
            if (csd[i - 1].categoryObjNameValue == csd[i].categoryObjNameValue) {
                willremoveIndex << i - 1 << i;
            }
        } else if ((csd[i - 1].actionType() == AddPannelActionType) &&
                   (csd[i].actionType() == RemovePannelActionType)) {
            if (csd[i - 1].pannelObjNameValue == csd[i].pannelObjNameValue) {
                willremoveIndex << i - 1 << i;
            }
        } else if ((csd[i - 1].actionType() == AddActionActionType) &&
                   (csd[i].actionType() == RemoveActionActionType)) {
            if (csd[i - 1].keyValue == csd[i].keyValue) {
                willremoveIndex << i - 1 << i;
            }
        }
    }
    res = remove_indexs(csd, willremoveIndex);
    willremoveIndex.clear();

    //! 筛选VisibleCategoryActionType，对于连续出现的操作只保留最后一步
    size = res.size();
    for (int i = 1; i < size; ++i) {
        if ((res[i - 1].actionType() == VisibleCategoryActionType) &&
            (res[i].actionType() == VisibleCategoryActionType)) {
            willremoveIndex << i - 1;//删除前一个只保留最后一个
        }
    }
    res = remove_indexs(res, willremoveIndex);
    willremoveIndex.clear();

    //! 针对RenameCategoryActionType和RenamePannelActionType操作，只需保留最后一个
    size = res.size();
    for (int i = 0; i < size; ++i) {
        if (res[i].actionType() == RenameCategoryActionType) {
            //向后查询，如果查询到有同一个Category改名，把这个索引加入删除队列
            for (int j = i + 1; j < size; ++j) {
                if ((res[j].actionType() == RenameCategoryActionType) &&
                    (res[i].categoryObjNameValue == res[j].categoryObjNameValue)) {
                    willremoveIndex << i;
                }
            }
        } else if (res[i].actionType() == RenamePannelActionType) {
            //向后查询，如果查询到有同一个pannel改名，把这个索引加入删除队列
            for (int j = i + 1; j < size; ++j) {
                if ((res[j].actionType() == RenamePannelActionType) &&
                    (res[i].pannelObjNameValue == res[j].pannelObjNameValue)) {
                    willremoveIndex << i;
                }
            }
        }
    }
    res = remove_indexs(res, willremoveIndex);
    willremoveIndex.clear();

    //! 针对连续的ChangeCategoryOrderActionType，ChangePannelOrderActionType，ChangeActionOrderActionType进行合并
    size = res.size();
    for (int i = 1; i < size; ++i) {
        if ((res[i - 1].actionType() == ChangeCategoryOrderActionType) &&
            (res[i].actionType() == ChangeCategoryOrderActionType) &&
            (res[i - 1].categoryObjNameValue == res[i].categoryObjNameValue)) {
            //说明连续两个顺序调整，把前一个indexvalue和后一个indexvalue相加，前一个删除
            res[i].indexValue += res[i - 1].indexValue;
            willremoveIndex << i - 1;
        } else if ((res[i - 1].actionType() == ChangePannelOrderActionType) &&
                   (res[i].actionType() == ChangePannelOrderActionType) &&
                   (res[i - 1].pannelObjNameValue == res[i].pannelObjNameValue)) {
            //说明连续两个顺序调整，把前一个indexvalue和后一个indexvalue相加，前一个删除
            res[i].indexValue += res[i - 1].indexValue;
            willremoveIndex << i - 1;
        } else if ((res[i - 1].actionType() == ChangeActionOrderActionType) &&
                   (res[i].actionType() == ChangeActionOrderActionType) &&
                   (res[i - 1].keyValue == res[i].keyValue)) {
            //说明连续两个顺序调整，把前一个indexvalue和后一个indexvalue相加，前一个删除
            res[i].indexValue += res[i - 1].indexValue;
            willremoveIndex << i - 1;
        }
    }
    res = remove_indexs(res, willremoveIndex);
    willremoveIndex.clear();

    //! 上一步操作可能会产生indexvalue为0的情况，此操作把indexvalue为0的删除
    size = res.size();
    for (int i = 0; i < size; ++i) {
        if ((res[i].actionType() == ChangeCategoryOrderActionType) ||
            (res[i].actionType() == ChangePannelOrderActionType) ||
            (res[i].actionType() == ChangeActionOrderActionType)) {
            if (0 == res[i].indexValue) {
                willremoveIndex << i;
            }
        }
    }
    res = remove_indexs(res, willremoveIndex);
    willremoveIndex.clear();
    return (res);
}



