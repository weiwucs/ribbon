//
// Created by weiwu on 21-5-2.
//

#include "RibbonActionsManager.h"
#include "RibbonMainWindow.h"

class RibbonActionsManagerPrivate {
public:
    RibbonActionsManager *mParent;
    QMap<int, QList<QAction *> > mTagToActions;     ///< tag : QList<QAction*>
    QMap<int, QString> mTagToName;                  ///< tag对应的名字
    QHash<QString, QAction *> mKeyToAction;         ///< key对应action
    QMap<QAction *, QString> mActionToKey;          ///< action对应key
    QMap<int, RibbonCategory *> mTagToCategory;   ///< 仅仅在autoRegisteActions函数会有用
    int mSale;                                      ///< 盐用于生成固定的id，在用户不主动设置key时，id基于msale生成，只要RibbonActionsManager的调用registeAction顺序不变，生成的id都不变，因为它是基于自增实现的
    RibbonActionsManagerPrivate(RibbonActionsManager *p);
};

RibbonActionsManagerPrivate::RibbonActionsManagerPrivate(RibbonActionsManager *p)
        : mParent(p), mSale(0) {
}


RibbonActionsManager::RibbonActionsManager(QObject *p) : QObject(p), m_d(new RibbonActionsManagerPrivate(this)) {
}


RibbonActionsManager::~RibbonActionsManager() {
    delete m_d;
}


/**
 * @brief 设置tag对应的名字，通过这个可以得到tag和文本的映射
 * @param tag
 * @param name
 * @note 在支持多语言的环境下，在语言切换时需要重新设置，以更新名字
 */
void RibbonActionsManager::setTagName(int tag, const QString &name) {
    m_d->mTagToName[tag] = name;
}


/**
 * @brief 获取tag对应的中文名字
 * @param tag
 * @return
 */
QString RibbonActionsManager::tagName(int tag) const {
    return (m_d->mTagToName.value(tag, ""));
}


/**
 * @brief 移除tag
 * @note 注意，这个函数非常耗时
 * @param tag
 */
void RibbonActionsManager::removeTag(int tag) {
    QList<QAction *> oldacts = actions(tag);

    //开始移除
    m_d->mTagToActions.remove(tag);
    m_d->mTagToName.remove(tag);
    //开始查找需要移出总表的action
    QList<QAction *> needRemoveAct;
    QList<QAction *> total;

    for (auto i = m_d->mTagToActions.begin(); i != m_d->mTagToActions.end(); ++i) {
        total += i.value();
    }
    for (QAction *a : oldacts) {
        if (!total.contains(a)) {
            needRemoveAct.append(a);
        }
    }
    //从总表移除action
    for (QAction *a:needRemoveAct) {
        auto i = m_d->mActionToKey.find(a);
        if (i != m_d->mActionToKey.end()) {
            m_d->mKeyToAction.remove(i.value());
            m_d->mActionToKey.erase(i);
        }
    }
}


/**
 * @brief 把action注册到管理器中，实现action的管理
 * @param act
 * @param tag tag是可以按照位进行叠加，见 @ref ActionTag 如果
 * 要定义自己的标签，建议定义大于@ref ActionTag::UserDefineActionTag 的值，
 * registeAction的tag是直接记录进去的，如果要多个标签并存，在registe之前先或好tag
 * @param key key是action对应的key，一个key只对应一个action，是查找action的关键
 * ,默认情况为一个QString(),这时key是QAction的objectName
 * @note 同一个action多次注册不同的tag可以通过tag索引到action，但通过action只能索引到最后一个注册的tag
 * @note tag的新增会触发actionTagChanged信号
 */
bool RibbonActionsManager::registeAction(QAction *act, int tag, const QString &key, bool enableEmit) {
    if (nullptr == act) {
        return (false);
    }
    QString k = key;

    if (k.isEmpty()) {
        k = QString("id_%1_%2").arg(m_d->mSale++).arg(act->objectName());
    }
    if (m_d->mKeyToAction.contains(k)) {
        qWarning() << "key: " << k
                   << " have been exist,you can set key in an unique value when use RibbonActionsManager::registeAction";
        return (false);
    }
    m_d->mKeyToAction[k] = act;
    m_d->mActionToKey[act] = k;
    //记录tag 对 action
    bool isneedemit = !(m_d->mTagToActions.contains(tag));//记录是否需要发射信号

    m_d->mTagToActions[tag].append(act);
    //绑定槽
    connect(act, &QObject::destroyed, this, &RibbonActionsManager::onActionDestroyed);
    if (isneedemit && enableEmit) {
        //说明新增tag
        emit actionTagChanged(tag, false);
    }
    return (true);
}


/**
 * @brief 取消action的注册
 *
 * 如果tag对应的最后一个action被撤销，tag也将一块删除
 * @param act
 * @note tag的删除会触发actionTagChanged信号
 * @note 如果action关联了多个tag，这些tag里的action都会被删除，对应的key也同理
 */
void RibbonActionsManager::unregisteAction(QAction *act, bool enableEmit) {
    if (nullptr == act) {
        return;
    }
    //绑定槽
    disconnect(act, &QObject::destroyed, this, &RibbonActionsManager::onActionDestroyed);
    removeAction(act, enableEmit);
}


/**
 * @brief 移除action
 *
 * 仅移除内存内容
 * @param act
 * @param enableEmit
 */
void RibbonActionsManager::removeAction(QAction *act, bool enableEmit) {
    QList<int> deletedTags;                         //记录删除的tag，用于触发actionTagChanged
    QMap<int, QList<QAction *> > tagToActions;      ///< tag : QList<QAction*>

    for (auto i = m_d->mTagToActions.begin(); i != m_d->mTagToActions.end(); ++i) {
        //把不是act的内容转移到tagToActions和tagToActionKeys中，之后再和m_d里的替换
        auto tmpi = tagToActions.insert(i.key(), QList<QAction *>());
        int count = 0;
        for (int j = 0; j < i.value().size(); ++j) {
            if (i.value()[j] != act) {
                tmpi.value().append(act);
                ++count;
            }
        }
        if (0 == count) {
            //说明这个tag没有内容
            tagToActions.erase(tmpi);
            deletedTags.append(i.key());
        }
    }
    //删除mKeyToAction
    QString key = m_d->mActionToKey.value(act);

    m_d->mActionToKey.remove(act);
    m_d->mKeyToAction.remove(key);

    //置换
    m_d->mTagToActions.swap(tagToActions);
    //发射信号
    if (enableEmit) {
        for (int tagdelete : deletedTags) {
            emit actionTagChanged(tagdelete, true);
        }
    }
}


/**
 * @brief 等同actions
 * @param tag
 * @return
 */
QList<QAction *> &RibbonActionsManager::filter(int tag) {
    return (actions(tag));
}


/**
 * @brief 根据tag得到actions
 * @param tag
 * @return
 */
QList<QAction *> &RibbonActionsManager::actions(int tag) {
    return (m_d->mTagToActions[tag]);
}


const QList<QAction *> RibbonActionsManager::actions(int tag) const {
    return (m_d->mTagToActions[tag]);
}


/**
 * @brief 获取所有的标签
 * @return
 */
QList<int> RibbonActionsManager::actionTags() const {
    return (m_d->mTagToActions.keys());
}


/**
 * @brief 通过key获取action
 * @param key
 * @return 如果没有key，返回nullptr
 */
QAction *RibbonActionsManager::action(const QString &key) const {
    return (m_d->mKeyToAction.value(key, nullptr));
}


/**
 * @brief 通过action找到key
 * @param act
 * @return 如果找不到，返回QString()
 */
QString RibbonActionsManager::key(QAction *act) const {
    return (m_d->mActionToKey.value(act, QString()));
}


/**
 * @brief 返回所有管理的action数
 * @return
 */
int RibbonActionsManager::count() const {
    return (m_d->mKeyToAction.size());
}


/**
 * @brief 返回所有管理的actions
 * @return
 */
QList<QAction *> RibbonActionsManager::allActions() const {
    return (m_d->mKeyToAction.values());
}


/**
 * @brief 自动加载RibbonMainWindow的action
 * 此函数会遍历@ref RibbonMainWindow 下的所有子object，找到action注册，
 * 并会遍历所有@ref RibbonCategory,把RibbonCategory下的action按RibbonCategory的title name进行分类
 *
 * 此函数会把所有category下的action生成tag并注册，返回的QMap<int, RibbonCategory *>是记录了category对应的tag
 *
 * 此函数还会把RibbonMainWindow下面的action，但不在任何一个category下的作为NotInRibbonCategoryTag标签注册，默认名字会赋予not in ribbon，
 * 可以通过@ref setTagName 改变
 *
 * @param w
 * @return
 * @note 此函数的调用最好在category设置了标题后调用，因为会以category的标题作为标签的命名
 */
QMap<int, RibbonCategory *> RibbonActionsManager::autoRegisteActions(RibbonMainWindow *w) {
    QMap<int, RibbonCategory *> res;
    //先遍历RibbonMainWindow下的所有子对象，把所有action找到
    QSet<QAction *> mainwindowActions;

    for (QObject *o : w->children()) {
        if (QAction *a = qobject_cast<QAction *>(o)) {
            //说明是action
            if (!a->objectName().isEmpty()) {
                mainwindowActions.insert(a);
            }
        }
    }
    //开始遍历每个category，加入action
    RibbonBar *bar = w->ribbonBar();

    if (nullptr == bar) {
        //非ribbon模式，直接退出
        return (res);
    }
    QSet<QAction *> categoryActions;
    QList<RibbonCategory *> categorys = bar->categoryPages();
    int tag = AutoCategoryDistinguishBeginTag;

    for (RibbonCategory *c : categorys) {
        QList<RibbonPannel *> pannels = c->pannelList();
        for (RibbonPannel *p : pannels) {
            categoryActions += autoRegisteWidgetActions(p, tag, false);
        }
        setTagName(tag, c->windowTitle());
        res[tag] = c;
        ++tag;
    }
    //找到不在功能区的actions
    QSet<QAction *> notincategory = mainwindowActions - categoryActions;

    for (QAction *a : notincategory) {
        if (!a->objectName().isEmpty()) {
            registeAction(a, NotInRibbonCategoryTag, a->objectName(), false);
        }
    }
    if (notincategory.size() > 0) {
        setTagName(NotInRibbonCategoryTag, tr("not in ribbon"));
    }
    for (auto i = res.begin(); i != res.end(); ++i) {
        connect(i.value(), &RibbonCategory::windowTitleChanged, this, &RibbonActionsManager::onCategoryTitleChanged);
    }
    m_d->mTagToCategory = res;
    return (res);
}


/**
 * @brief 自动加载action
 * @param w
 * @param tag
 * @param enableEmit
 * @return 返回成功加入RibbonActionsManager管理的action
 */
QSet<QAction *> RibbonActionsManager::autoRegisteWidgetActions(QWidget *w, int tag, bool enableEmit) {
    QSet<QAction *> res;
    QList<QAction *> was = w->actions();

    for (QAction *a : was) {
        if (res.contains(a) || a->objectName().isEmpty()) {
            //重复内容不重复加入
            //没有object name不加入
            continue;
        }
        if (registeAction(a, tag, a->objectName(), enableEmit)) {
            res.insert(a);
        }
    }
    return (res);
}


/**
 * @brief 根据标题查找action
 * @param text
 * @return
 */
QList<QAction *> RibbonActionsManager::search(const QString &text) {
    QList<QAction *> res;

    if (text.isEmpty()) {
        return (res);
    }
    QStringList kws = text.split(" ");

    if (kws.isEmpty()) {
        kws.append(text);
    }
    QList<QAction *> acts = m_d->mActionToKey.keys();

    for (const QString &k : kws) {
        for (auto i = m_d->mActionToKey.begin(); i != m_d->mActionToKey.end(); ++i) {
            if (i.key()->text().contains(k, Qt::CaseInsensitive)) {
                res.append(i.key());
            }
        }
    }
    return (res);
}


/**
 * @brief action 被delete时，将触发此槽把管理的action删除
 * @param o
 * @note 这个函数不会触发actionTagChanged信号
 */
void RibbonActionsManager::onActionDestroyed(QObject *o) {
    QAction *act = static_cast<QAction *>(o);

    removeAction(act, false);
}


/**
 * @brief autoRegisteActions函数会关联此槽，在标签内容改变时改变tag 对应 文本
 * @param title
 */
void RibbonActionsManager::onCategoryTitleChanged(const QString &title) {
    RibbonCategory *c = qobject_cast<RibbonCategory *>(sender());

    if (nullptr == c) {
        return;
    }
    int tag = m_d->mTagToCategory.key(c, -1);

    if (tag == -1) {
        return;
    }
    setTagName(tag, title);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RibbonActionsModel
////////////////////////////////////////////////////////////////////////////////////////////////////////
class RibbonActionsModelPrivate {
public:
    RibbonActionsManagerModel *mParent;
    RibbonActionsManager *mMgr;
    int mTag;
    QString mSeatchText;
    QList<QAction *> mActions;

    RibbonActionsModelPrivate(RibbonActionsManagerModel *m);

    void updateRef();

    int count() const;

    QAction *at(int index);

    bool isNull() const;
};

RibbonActionsModelPrivate::RibbonActionsModelPrivate(RibbonActionsManagerModel *m)
        : mParent(m), mMgr(nullptr), mTag(RibbonActionsManager::CommonlyUsedActionTag), mSeatchText(QString()) {
}


void RibbonActionsModelPrivate::updateRef() {
    if (isNull()) {
        return;
    }
    if (!mSeatchText.isEmpty()) {
        mActions = mMgr->search(mSeatchText);
    } else {
        mActions = mMgr->actions(mTag);
    }
}


int RibbonActionsModelPrivate::count() const {
    if (isNull()) {
        return (0);
    }
    return (mActions.size());
}


QAction *RibbonActionsModelPrivate::at(int index) {
    if (isNull()) {
        return (nullptr);
    }
    if (index >= count()) {
        return (nullptr);
    }
    return (mActions.at(index));
}


bool RibbonActionsModelPrivate::isNull() const {
    return (mMgr == nullptr);
}


RibbonActionsManagerModel::RibbonActionsManagerModel(QObject *p) : QAbstractListModel(p),
                                                                   m_d(new RibbonActionsModelPrivate(this)) {
}


RibbonActionsManagerModel::RibbonActionsManagerModel(RibbonActionsManager *m, QObject *p) : QAbstractListModel(p),
                                                                                            m_d(new RibbonActionsModelPrivate(this)) {
    setupActionsManager(m);
}


RibbonActionsManagerModel::~RibbonActionsManagerModel() {
    delete m_d;
}


int RibbonActionsManagerModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {//非顶层
        return (0);
    }
    //顶层
    return (m_d->count());
}


QVariant RibbonActionsManagerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    Q_UNUSED(section);
    if (role != Qt::DisplayRole) {
        return (QVariant());
    }
    if (Qt::Horizontal == orientation) {
        return (tr("action name"));
    }
    return (QVariant());
}


Qt::ItemFlags RibbonActionsManagerModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return (Qt::NoItemFlags);
    }
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


QVariant RibbonActionsManagerModel::data(const QModelIndex &index, int role) const {
    QAction *act = indexToAction(index);

    if (nullptr == act) {
        return (QVariant());
    }

    switch (role) {
        case Qt::DisplayRole:
            return (act->text());

        case Qt::DecorationRole:
            return (act->icon());

        default:
            break;
    }
    return (QVariant());
}


void RibbonActionsManagerModel::setFilter(int tag) {
    m_d->mTag = tag;
    update();
}


void RibbonActionsManagerModel::update() {
    beginResetModel();
    m_d->updateRef();
    endResetModel();
}


void RibbonActionsManagerModel::setupActionsManager(RibbonActionsManager *m) {
    m_d->mMgr = m;
    m_d->mTag = RibbonActionsManager::CommonlyUsedActionTag;
    m_d->mActions = m->filter(m_d->mTag);
    connect(m, &RibbonActionsManager::actionTagChanged, this, &RibbonActionsManagerModel::onActionTagChanged);
    update();
}


void RibbonActionsManagerModel::uninstallActionsManager() {
    if (!m_d->isNull()) {
        disconnect(m_d->mMgr, &RibbonActionsManager::actionTagChanged, this,
                   &RibbonActionsManagerModel::onActionTagChanged);
        m_d->mMgr = nullptr;
        m_d->mTag = RibbonActionsManager::CommonlyUsedActionTag;
    }
    update();
}


QAction *RibbonActionsManagerModel::indexToAction(QModelIndex index) const {
    if (!index.isValid()) {
        return (nullptr);
    }
    if (index.row() >= m_d->count()) {
        return (nullptr);
    }
    return (m_d->at(index.row()));
}


/**
 * @brief 查找
 * @param text
 */
void RibbonActionsManagerModel::search(const QString &text) {
    m_d->mSeatchText = text;
    update();
}


void RibbonActionsManagerModel::onActionTagChanged(int tag, bool isdelete) {
    if (isdelete && (tag == m_d->mTag)) {
        m_d->mTag = RibbonActionsManager::UnknowActionTag;
        update();
    } else {
        if (tag == m_d->mTag) {
            update();
        }
    }
}

