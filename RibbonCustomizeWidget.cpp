//
// Created by weiwu on 21-5-2.
//

#include "RibbonCustomizeWidget.h"
#include "RibbonCustomizeData.h"


bool sa_customize_datas_to_xml(QXmlStreamWriter *xml, const QList<RibbonCustomizeData> &cds) {
    if (cds.size() <= 0) {
        return (false);
    }

    xml->writeStartElement("sa-ribbon-customize");
    for (const RibbonCustomizeData &d : cds) {
        xml->writeStartElement("customize-data");
        xml->writeAttribute("type", QString::number(d.actionType()));
        xml->writeAttribute("index", QString::number(d.indexValue));
        xml->writeAttribute("key", d.keyValue);
        xml->writeAttribute("category", d.categoryObjNameValue);
        xml->writeAttribute("pannel", d.pannelObjNameValue);
        xml->writeAttribute("row-prop", QString::number(d.actionRowProportionValue));
        xml->writeEndElement();
    }
    xml->writeEndElement();
    if (xml->hasError()) {
        qWarning() << "write has error";
    }
    return (true);
}


QList<RibbonCustomizeData> sa_customize_datas_from_xml(QXmlStreamReader *xml, RibbonActionsManager *mgr) {
    //先找到"sa-ribbon-customize"
    while (!xml->atEnd()) {
        qDebug() << "name:" << xml->name() << " qualifiedName:" << xml->qualifiedName();

        if (xml->isStartElement() && (xml->name() == "sa-ribbon-customize")) {
            break;
        }
        xml->readNext();
    }
    QList<RibbonCustomizeData> res;

    //开始遍历"customize-data"
    while (!xml->atEnd()) {
        if (xml->isStartElement() && (xml->name() == "customize-data")) {
            //首先读取属性type
            RibbonCustomizeData d;
            QXmlStreamAttributes attrs = xml->attributes();
            if (!attrs.hasAttribute("type")) {
                //说明异常，跳过这个
                xml->readNextStartElement();
                continue;
            }
            bool isOk = false;
            int v = xml->attributes().value("type").toInt(&isOk);
            if (!isOk) {
                //说明异常，跳过这个
                xml->readNextStartElement();
                continue;
            }
            d.setActionType(static_cast<RibbonCustomizeData::ActionType>(v));
            //开始读取子对象
            if (attrs.hasAttribute("index")) {
                v = xml->attributes().value("index").toInt(&isOk);
                if (isOk) {
                    d.indexValue = v;
                }
            }
            if (attrs.hasAttribute("key")) {
                d.keyValue = attrs.value("key").toString();
            }
            if (attrs.hasAttribute("category")) {
                d.categoryObjNameValue = attrs.value("category").toString();
            }
            if (attrs.hasAttribute("pannel")) {
                d.pannelObjNameValue = attrs.value("pannel").toString();
            }
            if (attrs.hasAttribute("row-prop")) {
                v = xml->attributes().value("row-prop").toInt(&isOk);
                if (isOk) {
                    d.actionRowProportionValue = static_cast<RibbonPannelItem::RowProportion>(v);
                }
            }
            d.setActionsManager(mgr);
            res.append(d);
        }
        xml->readNext();
    }
    if (xml->hasError()) {
        qWarning() << xml->errorString();
    }
    return (res);
}


int sa_customize_datas_apply(const QList<RibbonCustomizeData> &cds, RibbonMainWindow *w) {
    int c = 0;

    for (const RibbonCustomizeData &d : cds) {
        if (d.apply(w)) {
            ++c;
        }
    }
    return (c);
}


bool sa_apply_customize_from_xml_file(const QString &filePath, RibbonMainWindow *w, RibbonActionsManager *mgr) {
    QFile f(filePath);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return (false);
    }
    f.seek(0);
    QXmlStreamReader xml(&f);

    return (RibbonCustomizeWidget::fromXml(&xml, w, mgr));
}


/**
 * @brief 构建RibbonCustomizeWidget的Ui
 */
class RibbonCustomizeWidgetUi {
public:
    QHBoxLayout *horizontalLayoutMain;
    QVBoxLayout *verticalLayoutSelect;
    QLabel *labelSelectAction;
    QHBoxLayout *horizontalLayoutSearch;
    QComboBox *comboBoxActionIndex;
    QLineEdit *lineEditSearchAction;
    QListView *listViewSelect;
    QVBoxLayout *verticalLayoutMidButtons;
    QSpacerItem *verticalSpacerUp;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonDelete;
    QSpacerItem *verticalSpacerDown;
    QLabel *labelProportion;
    QComboBox *comboBoxActionProportion;
    QVBoxLayout *verticalLayoutResult;
    QLabel *labelCustomize;
    QHBoxLayout *horizontalLayoutCategorySelect;
    QRadioButton *radioButtonMainCategory;
    QRadioButton *radioButtonAllCategory;
    QButtonGroup *radioButtonGroup;
    QTreeView *treeViewResult;
    QHBoxLayout *horizontalLayoutActionOptBtns;
    QPushButton *pushButtonNewCategory;
    QPushButton *pushButtonNewPannel;
    QPushButton *pushButtonRename;
    QVBoxLayout *verticalLayoutRightButtons;
    QSpacerItem *verticalSpacerUp2;
    QToolButton *toolButtonUp;
    QToolButton *toolButtonDown;
    QSpacerItem *verticalSpacerDown2;

    void setupUi(QWidget *customizeWidget) {
        if (customizeWidget->objectName().isEmpty()) {
            customizeWidget->setObjectName(QStringLiteral("RibbonCustomizeWidget"));
        }
        customizeWidget->resize(800, 600);
        horizontalLayoutMain = new QHBoxLayout(customizeWidget);
        horizontalLayoutMain->setObjectName(QStringLiteral("horizontalLayoutMain"));
        verticalLayoutSelect = new QVBoxLayout();
        verticalLayoutSelect->setObjectName(QStringLiteral("verticalLayoutSelect"));
        labelSelectAction = new QLabel(customizeWidget);
        labelSelectAction->setObjectName(QStringLiteral("labelSelectAction"));

        verticalLayoutSelect->addWidget(labelSelectAction);

        horizontalLayoutSearch = new QHBoxLayout();
        horizontalLayoutSearch->setObjectName(QStringLiteral("horizontalLayoutSearch"));
        comboBoxActionIndex = new QComboBox(customizeWidget);
        comboBoxActionIndex->setObjectName(QStringLiteral("comboBoxActionIndex"));
        comboBoxActionIndex->setEditable(false);

        horizontalLayoutSearch->addWidget(comboBoxActionIndex);

        lineEditSearchAction = new QLineEdit(customizeWidget);
        lineEditSearchAction->setObjectName(QStringLiteral("lineEditSearchAction"));

        horizontalLayoutSearch->addWidget(lineEditSearchAction);


        verticalLayoutSelect->addLayout(horizontalLayoutSearch);

        listViewSelect = new QListView(customizeWidget);
        listViewSelect->setObjectName(QStringLiteral("listViewSelect"));

        verticalLayoutSelect->addWidget(listViewSelect);


        horizontalLayoutMain->addLayout(verticalLayoutSelect);

        verticalLayoutMidButtons = new QVBoxLayout();
        verticalLayoutMidButtons->setObjectName(QStringLiteral("verticalLayoutMidButtons"));
        verticalSpacerUp = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayoutMidButtons->addItem(verticalSpacerUp);

        pushButtonAdd = new QPushButton(customizeWidget);
        pushButtonAdd->setObjectName(QStringLiteral("pushButtonAdd"));
        pushButtonAdd->setEnabled(false);

        verticalLayoutMidButtons->addWidget(pushButtonAdd);

        pushButtonDelete = new QPushButton(customizeWidget);
        pushButtonDelete->setObjectName(QStringLiteral("pushButtonDelete"));
        pushButtonDelete->setEnabled(false);

        verticalLayoutMidButtons->addWidget(pushButtonDelete);

        verticalSpacerDown = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayoutMidButtons->addItem(verticalSpacerDown);

        labelProportion = new QLabel(customizeWidget);
        labelProportion->setObjectName(QStringLiteral("labelProportion"));
        verticalLayoutMidButtons->addWidget(labelProportion);

        comboBoxActionProportion = new QComboBox(customizeWidget);
        comboBoxActionProportion->setObjectName(QStringLiteral("comboBoxActionProportion"));
        comboBoxActionProportion->setEditable(false);
        verticalLayoutMidButtons->addWidget(comboBoxActionProportion);
        horizontalLayoutMain->addLayout(verticalLayoutMidButtons);

        verticalLayoutResult = new QVBoxLayout();
        verticalLayoutResult->setObjectName(QStringLiteral("verticalLayoutResult"));
        labelCustomize = new QLabel(customizeWidget);
        labelCustomize->setObjectName(QStringLiteral("labelCustomize"));

        verticalLayoutResult->addWidget(labelCustomize);

        horizontalLayoutCategorySelect = new QHBoxLayout();
        horizontalLayoutCategorySelect->setObjectName(QStringLiteral("horizontalLayoutCategorySelect"));
        radioButtonMainCategory = new QRadioButton(customizeWidget);
        radioButtonMainCategory->setObjectName(QStringLiteral("radioButtonMainCategory"));
        radioButtonMainCategory->setChecked(false);

        horizontalLayoutCategorySelect->addWidget(radioButtonMainCategory);

        radioButtonAllCategory = new QRadioButton(customizeWidget);
        radioButtonAllCategory->setObjectName(QStringLiteral("radioButtonAllCategory"));
        radioButtonAllCategory->setChecked(true);

        horizontalLayoutCategorySelect->addWidget(radioButtonAllCategory);

        radioButtonGroup = new QButtonGroup(customizeWidget);
        radioButtonGroup->addButton(radioButtonMainCategory);
        radioButtonGroup->addButton(radioButtonAllCategory);

        verticalLayoutResult->addLayout(horizontalLayoutCategorySelect);

        treeViewResult = new QTreeView(customizeWidget);
        treeViewResult->setObjectName(QStringLiteral("treeViewResult"));
        treeViewResult->setHeaderHidden(true);
        treeViewResult->setSelectionMode(QAbstractItemView::SingleSelection);
        treeViewResult->setAnimated(true);                                      //支持动画
        treeViewResult->setEditTriggers(QAbstractItemView::NoEditTriggers);     //不允许直接在item上重命名


        verticalLayoutResult->addWidget(treeViewResult);

        horizontalLayoutActionOptBtns = new QHBoxLayout();
        horizontalLayoutActionOptBtns->setObjectName(QStringLiteral("horizontalLayoutActionOptBtns"));
        pushButtonNewCategory = new QPushButton(customizeWidget);
        pushButtonNewCategory->setObjectName(QStringLiteral("pushButtonNewCategory"));

        horizontalLayoutActionOptBtns->addWidget(pushButtonNewCategory);

        pushButtonNewPannel = new QPushButton(customizeWidget);
        pushButtonNewPannel->setObjectName(QStringLiteral("pushButtonNewPannel"));

        horizontalLayoutActionOptBtns->addWidget(pushButtonNewPannel);

        pushButtonRename = new QPushButton(customizeWidget);
        pushButtonRename->setObjectName(QStringLiteral("pushButtonRename"));

        horizontalLayoutActionOptBtns->addWidget(pushButtonRename);


        verticalLayoutResult->addLayout(horizontalLayoutActionOptBtns);


        horizontalLayoutMain->addLayout(verticalLayoutResult);

        verticalLayoutRightButtons = new QVBoxLayout();
        verticalLayoutRightButtons->setObjectName(QStringLiteral("verticalLayoutRightButtons"));
        verticalSpacerUp2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayoutRightButtons->addItem(verticalSpacerUp2);

        toolButtonUp = new QToolButton(customizeWidget);
        toolButtonUp->setObjectName(QStringLiteral("pushButtonUp"));
        toolButtonUp->setArrowType(Qt::UpArrow);
        toolButtonUp->setAutoRaise(true);

        verticalLayoutRightButtons->addWidget(toolButtonUp);

        toolButtonDown = new QToolButton(customizeWidget);
        toolButtonDown->setObjectName(QStringLiteral("pushButtonDown"));
        toolButtonDown->setArrowType(Qt::DownArrow);
        toolButtonDown->setAutoRaise(true);

        verticalLayoutRightButtons->addWidget(toolButtonDown);

        verticalSpacerDown2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayoutRightButtons->addItem(verticalSpacerDown2);


        horizontalLayoutMain->addLayout(verticalLayoutRightButtons);

        retranslateUi(customizeWidget);
    } // setupUi


    void retranslateUi(QWidget *customizeWidget) {
        customizeWidget->setWindowTitle(
                QApplication::translate("RibbonCustomizeWidget", "Customize Widget", Q_NULLPTR));
        labelSelectAction->setText(QApplication::translate("RibbonCustomizeWidget",
                                                           "\344\273\216\344\270\213\345\210\227\351\200\211\351\241\271\345\215\241\351\200\211\346\213\251\345\221\275\344\273\244\357\274\232",
                                                           Q_NULLPTR));
        lineEditSearchAction->setInputMask(QString());
        lineEditSearchAction->setText(QString());
        lineEditSearchAction->setPlaceholderText(
                QApplication::translate("RibbonCustomizeWidget", "\346\237\245\346\211\276\345\221\275\344\273\244",
                                        Q_NULLPTR));
        pushButtonAdd->setText(
                QApplication::translate("RibbonCustomizeWidget", "\346\267\273\345\212\240 >>", Q_NULLPTR));
        pushButtonDelete->setText(
                QApplication::translate("RibbonCustomizeWidget", "<< \345\210\240\351\231\244", Q_NULLPTR));
        labelCustomize->setText(QApplication::translate("RibbonCustomizeWidget",
                                                        "\350\207\252\345\256\232\344\271\211\345\212\237\350\203\275\345\214\272\357\274\232",
                                                        Q_NULLPTR));
        radioButtonMainCategory->setText(
                QApplication::translate("RibbonCustomizeWidget", "\344\270\273\351\200\211\351\241\271\345\215\241",
                                        Q_NULLPTR));
        radioButtonAllCategory->setText(QApplication::translate("RibbonCustomizeWidget",
                                                                "\346\211\200\346\234\211\351\200\211\351\241\271\345\215\241",
                                                                Q_NULLPTR));
        pushButtonNewCategory->setText(QApplication::translate("RibbonCustomizeWidget",
                                                               "\346\226\260\345\273\272\351\200\211\351\241\271\345\215\241",
                                                               Q_NULLPTR));
        pushButtonNewPannel->setText(
                QApplication::translate("RibbonCustomizeWidget", "\346\226\260\345\273\272\347\273\204", Q_NULLPTR));
        pushButtonRename->setText(
                QApplication::translate("RibbonCustomizeWidget", "\351\207\215\345\221\275\345\220\215", Q_NULLPTR));
        labelProportion->setText(QApplication::translate("RibbonCustomizeWidget", "proportion:", Q_NULLPTR));
    } // retranslateUi
};

/**
 * @brief 管理RibbonCustomizeWidget的业务逻辑
 */
class RibbonCustomizeWidgetPrivate {
public:
    RibbonCustomizeWidget *mParent;
    RibbonCustomizeWidget::RibbonTreeShowType mShowType;  ///< 显示类型
    RibbonMainWindow *mRibbonWindow;                      ///< 保存RibbonMainWindow的指针
    RibbonActionsManager *mActionMgr;                     ///< action管理器
    RibbonActionsManagerModel *mAcionModel;               ///< action管理器对应的model
    QStandardItemModel *mRibbonModel;                       ///< 用于很成ribbon的树
    int mCustomizeCategoryCount;                            ///< 记录自定义Category的个数
    int mCustomizePannelCount;                              ///< 记录自定义Pannel的个数
    RibbonCustomizeWidgetPrivate(RibbonCustomizeWidget *p);

    void updateModel();

    QList<RibbonCustomizeData> mCustomizeDatas;           ///< 记录所有的自定义动作
    QList<RibbonCustomizeData> mOldCustomizeDatas;        ///< 记录旧的自定义动作
    //创建一个随机id，形如：pre_QDateTime::currentMSecsSinceEpoch_suf
    static QString makeRandomObjName(const QString &pre);

    int itemLevel(QStandardItem *item) const;

    //
    bool isCustomizeItem(QStandardItem *item) const;

    //把item转换为category
    RibbonCategory *itemToCategory(QStandardItem *item) const;

    //把item转换为RibbonPannel
    RibbonPannel *itemToPannel(QStandardItem *item) const;

    //获取item对应的object name
    QString itemObjectName(QStandardItem *item) const;

    //判断是否可以自定义
    bool isItemCanCustomize(QStandardItem *item) const;

    //从item转为action
    QAction *itemToAction(QStandardItem *item) const;
};

RibbonCustomizeWidgetPrivate::RibbonCustomizeWidgetPrivate(RibbonCustomizeWidget *p)
        : mParent(p), mShowType(RibbonCustomizeWidget::ShowAllCategory), mRibbonWindow(nullptr), mActionMgr(nullptr),
          mAcionModel(new RibbonActionsManagerModel(p)), mRibbonModel(new QStandardItemModel(p)),
          mCustomizeCategoryCount(0), mCustomizePannelCount(0) {
}


void RibbonCustomizeWidgetPrivate::updateModel() {
    if (mRibbonWindow == nullptr) {
        return;
    }
    mRibbonModel->clear();
    RibbonBar *ribbonbar = mRibbonWindow->ribbonBar();
    QList<RibbonCategory *> categorys = ribbonbar->categoryPages();

    for (RibbonCategory *c : categorys) {
        if ((mShowType == RibbonCustomizeWidget::ShowMainCategory) && c->isContextCategory()) {
            //如果是只显示主内容，如果是上下文标签就忽略
            continue;
        }
        QStandardItem *ci = new QStandardItem();
        if (c->isContextCategory()) {
            ci->setText(QString("[%1]").arg(c->windowTitle()));
        } else {
            ci->setText(c->windowTitle());
        }
        if (c->isCanCustomize() && !c->isContextCategory()) {
            //上下文标签不做显示隐藏处理
            ci->setCheckable(true);
            ci->setCheckState(ribbonbar->isCategoryVisible(c) ? Qt::Checked : Qt::Unchecked);
            ci->setData(true, RibbonCustomizeWidget::CanCustomizeRole);//标记这个是可以自定义的
        }
        ci->setData(0, RibbonCustomizeWidget::LevelRole);
        ci->setData(QVariant::fromValue<qintptr>(qintptr(c)), RibbonCustomizeWidget::PointerRole);
        QList<RibbonPannel *> pannels = c->pannelList();
        for (RibbonPannel *p : pannels) {
            QStandardItem *pi = new QStandardItem(p->windowTitle());
            pi->setData(1, RibbonCustomizeWidget::LevelRole);
            pi->setData(QVariant::fromValue<qintptr>(qintptr(p)), RibbonCustomizeWidget::PointerRole);
            if (p->isCanCustomize()) {
                pi->setData(true, RibbonCustomizeWidget::CanCustomizeRole);//标记这个是可以自定义的
            }
            ci->appendRow(pi);
            const QList<RibbonPannelItem *> &items = p->ribbonPannelItem();
            for (RibbonPannelItem *i : items) {
                if (i->action->isSeparator()) {
                    continue;
                }
                QStandardItem *ii = new QStandardItem();
                if (i->customWidget) {
                    //如果是自定义窗口
                    if (i->widget()->windowTitle().isEmpty() && i->widget()->windowIcon().isNull()) {
                        delete ii;
                        continue;//如果窗口啥也没有，就跳过
                    }
                    ii->setText(i->widget()->windowTitle());
                    ii->setIcon(i->widget()->windowIcon());
                    if (RibbonCustomizeData::isCanCustomize(i->widget())) {
                        ii->setData(true, RibbonCustomizeWidget::CanCustomizeRole);//标记这个是可以自定义的
                    }
                } else {
                    ii->setText(i->action->text());
                    ii->setIcon(i->action->icon());
                    if (RibbonCustomizeData::isCanCustomize(i->action)) {
                        ii->setData(true, RibbonCustomizeWidget::CanCustomizeRole);//标记这个是可以自定义的
                    }
                }
                ii->setData(2, RibbonCustomizeWidget::LevelRole);
                ii->setData(QVariant::fromValue<qintptr>(qintptr(i)), RibbonCustomizeWidget::PointerRole);
                pi->appendRow(ii);
            }
        }
        mRibbonModel->appendRow(ci);
    }
}


/**
 * @brief 创建一个随机id，形如：pre_QDateTime::currentMSecsSinceEpoch
 * @param pre 前缀
 * @return
 */
QString RibbonCustomizeWidgetPrivate::makeRandomObjName(const QString &pre) {
    return (QString("%1_%2").arg(pre).arg(QDateTime::currentMSecsSinceEpoch()));
}


/**
 * @brief 获取item的level
 * @param item
 * @return
 */
int RibbonCustomizeWidgetPrivate::itemLevel(QStandardItem *item) const {
    return (item->data(RibbonCustomizeWidget::LevelRole).toInt());
}


/**
 * @brief 判断itemn为自定义的item，自定义的item都带有CustomizeRole角色
 * @param item
 * @return
 */
bool RibbonCustomizeWidgetPrivate::isCustomizeItem(QStandardItem *item) const {
    if (nullptr == item) {
        return (false);
    }
    return (item->data(RibbonCustomizeWidget::CustomizeRole).isValid());
}


/**
 * @brief 把item转换为category
 * @param item
 * @return无法转换返回nullptr
 */
RibbonCategory *RibbonCustomizeWidgetPrivate::itemToCategory(QStandardItem *item) const {
    int level = item->data(RibbonCustomizeWidget::LevelRole).toInt();

    if (level != 0) {
        return (nullptr);
    }
    qintptr p = item->data(RibbonCustomizeWidget::PointerRole).value<qintptr>();

    return (reinterpret_cast<RibbonCategory *>(p));
}


/**
 * @brief 把item转换为pannel
 * @param item
 * @return 无法转换返回nullptr
 */
RibbonPannel *RibbonCustomizeWidgetPrivate::itemToPannel(QStandardItem *item) const {
    int level = item->data(RibbonCustomizeWidget::LevelRole).toInt();

    if (level != 1) {
        return (nullptr);
    }
    qintptr p = item->data(RibbonCustomizeWidget::PointerRole).value<qintptr>();

    return (reinterpret_cast<RibbonPannel *>(p));
}


/**
 * @brief 获取item对应的objectname
 * @param item
 * @return 如果无法获取，返回一个空的QString
 */
QString RibbonCustomizeWidgetPrivate::itemObjectName(QStandardItem *item) const {
    QString objName;

    if (isCustomizeItem(item)) {
        //说明是自定义的
        objName = item->data(RibbonCustomizeWidget::CustomizeObjNameRole).toString();
    } else {
        //说明这个是非自定义的
        int level = itemLevel(item);
        if (0 == level) {
            RibbonCategory *category = itemToCategory(item);
            if (category) {
                objName = category->objectName();
            }
        } else if (1 == level) {
            RibbonPannel *pannel = itemToPannel(item);
            if (pannel) {
                objName = pannel->objectName();
            }
        }
    }
    return (objName);
}


/**
 * @brief 判断item是否可自定义
 * @param item
 * @return
 */
bool RibbonCustomizeWidgetPrivate::isItemCanCustomize(QStandardItem *item) const {
    if (nullptr == item) {
        return (false);
    }
    QVariant v = item->data(RibbonCustomizeWidget::CanCustomizeRole);

    if (v.isValid()) {
        return (v.toBool());
    }
    return (false);
}


/**
 * @brief 从item 转为action
 * @param item
 * @return
 */
QAction *RibbonCustomizeWidgetPrivate::itemToAction(QStandardItem *item) const {
    if (2 != itemLevel(item)) {
        return (nullptr);
    }
    //这里要非常注意，RibbonCustomizeWidget::CustomizeRole为true时，说明这个是自定义的内容，
    //这时PointerRole里存放的是action指针，不是RibbonPannelItem
    QAction *act = nullptr;

    if (item->data(RibbonCustomizeWidget::CustomizeRole).toBool()) {
        act = reinterpret_cast<QAction *>(item->data(RibbonCustomizeWidget::PointerRole).value<qintptr>());
    } else {
        RibbonPannelItem *pi = reinterpret_cast<RibbonPannelItem *>(item->data(
                RibbonCustomizeWidget::PointerRole).value<qintptr>());
        act = (pi->action);
    }
    return (act);
}


/**
 * @brief RibbonCustomizeWidget::RibbonCustomizeWidget
 * @param ribbonWindow 传入需要管理的RibbonMainWindow指针
 * @param parent 用于界面生成的parent，可以和RibbonMainWindow一样
 * @param f 同QWidget::QWidget的第二个参数
 */
RibbonCustomizeWidget::RibbonCustomizeWidget(RibbonMainWindow *ribbonWindow, QWidget *parent, Qt::WindowFlags f)
        : QWidget(parent, f), ui(new RibbonCustomizeWidgetUi), m_d(new RibbonCustomizeWidgetPrivate(this)) {
    m_d->mRibbonWindow = ribbonWindow;

    ui->setupUi(this);
    ui->listViewSelect->setModel(m_d->mAcionModel);
    ui->treeViewResult->setModel(m_d->mRibbonModel);
    initConnection();
    updateModel();
}


RibbonCustomizeWidget::~RibbonCustomizeWidget() {
    delete ui;
    delete m_d;
}


void RibbonCustomizeWidget::initConnection() {
//    这个需要qt5.8以上支持
//    connect(ui->comboBoxActionIndex, QOverload<int>::of(&QComboBox::currentIndexChanged)
//        , this, &RibbonCustomizeWidget::onComboBoxActionIndexCurrentIndexChanged);
    connect(ui->comboBoxActionIndex, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &RibbonCustomizeWidget::onComboBoxActionIndexCurrentIndexChanged);
//    这个需要qt5.8以上支持
//    connect(ui->radioButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked)
//        , this, &RibbonCustomizeWidget::onRadioButtonGroupButtonClicked);
    connect(ui->radioButtonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
            this, &RibbonCustomizeWidget::onRadioButtonGroupButtonClicked);
    connect(ui->pushButtonNewCategory, &QPushButton::clicked, this,
            &RibbonCustomizeWidget::onPushButtonNewCategoryClicked);
    connect(ui->pushButtonNewPannel, &QPushButton::clicked, this, &RibbonCustomizeWidget::onPushButtonNewPannelClicked);
    connect(ui->pushButtonRename, &QPushButton::clicked, this, &RibbonCustomizeWidget::onPushButtonRenameClicked);
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &RibbonCustomizeWidget::onPushButtonAddClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &RibbonCustomizeWidget::onPushButtonDeleteClicked);
    connect(ui->listViewSelect, &QAbstractItemView::clicked, this, &RibbonCustomizeWidget::onListViewSelectClicked);
    connect(ui->treeViewResult, &QAbstractItemView::clicked, this, &RibbonCustomizeWidget::onTreeViewResultClicked);
    connect(ui->toolButtonUp, &QToolButton::clicked, this, &RibbonCustomizeWidget::onToolButtonUpClicked);
    connect(ui->toolButtonDown, &QToolButton::clicked, this, &RibbonCustomizeWidget::onToolButtonDownClicked);
    connect(m_d->mRibbonModel, &QStandardItemModel::itemChanged, this, &RibbonCustomizeWidget::onItemChanged);
    connect(ui->lineEditSearchAction, &QLineEdit::textEdited, this,
            &RibbonCustomizeWidget::onLineEditSearchActionTextEdited);
}


/**
 * @brief 设置action管理器
 * @param mgr
 */
void RibbonCustomizeWidget::setupActionsManager(RibbonActionsManager *mgr) {
    m_d->mActionMgr = mgr;
    if (m_d->mActionMgr) {
        m_d->mAcionModel->uninstallActionsManager();
    }
    m_d->mAcionModel->setupActionsManager(mgr);
    //更新左边复选框
    QList<int> tags = mgr->actionTags();

    ui->comboBoxActionIndex->clear();
    for (int tag : tags) {
        ui->comboBoxActionIndex->addItem(mgr->tagName(tag), tag);
    }
}


bool RibbonCustomizeWidget::isChanged() const {
    return (m_d->mCustomizeDatas.size() > 0);
}


/**
 * @brief 获取model
 * @return
 */
const QStandardItemModel *RibbonCustomizeWidget::model() const {
    return (m_d->mRibbonModel);
}


/**
 * @brief 根据当前的radiobutton选项来更新model
 */
void RibbonCustomizeWidget::updateModel() {
    updateModel(ui->radioButtonAllCategory->isChecked() ? ShowAllCategory : ShowMainCategory);
    if (m_d->mRibbonWindow) {
        RibbonBar *bar = m_d->mRibbonWindow->ribbonBar();
        if (bar) {
            ui->comboBoxActionProportion->clear();
            if (bar->isTwoRowStyle()) {
                ui->comboBoxActionProportion->addItem(tr("large"), RibbonPannelItem::Large);
                ui->comboBoxActionProportion->addItem(tr("small"), RibbonPannelItem::Small);
            } else {
                ui->comboBoxActionProportion->addItem(tr("large"), RibbonPannelItem::Large);
                ui->comboBoxActionProportion->addItem(tr("medium"), RibbonPannelItem::Medium);
                ui->comboBoxActionProportion->addItem(tr("small"), RibbonPannelItem::Small);
            }
        }
    }
}


/**
 * @brief 更新model
 */
void RibbonCustomizeWidget::updateModel(RibbonTreeShowType type) {
    m_d->mShowType = type;
    m_d->updateModel();
}


/**
 * @brief 应用所有设定
 * @return 应用成功返回true
 * @note 所有设定有一个应用成功都会返回true
 */
bool RibbonCustomizeWidget::applys() {
    simplify();
    return (sa_customize_datas_apply(m_d->mCustomizeDatas, m_d->mRibbonWindow) > 0);
}


/**
 * @brief 转换为xml
 *
 * 此函数仅会写element，不会写document相关内容，因此如果需要写document，
 * 需要在此函数前调用QXmlStreamWriter::writeStartDocument(),在此函数后调用QXmlStreamWriter::writeEndDocument()
 *
 * @note 注意，在传入QXmlStreamWriter之前，需要设置编码为utf-8:xml->setCodec("utf-8");
 * @note 由于QXmlStreamWriter在QString作为io时，是不支持编码的，而此又无法保证自定义过程不出现中文字符，
 * 因此，QXmlStreamWriter不应该通过QString进行构造，如果需要用到string，也需要通过QByteArray构造，如：
 * @code
 * RibbonCustomizeDialog dlg(this);//this为RibbonMainWindow的窗口
 * dlg.setupActionsManager(m_actMgr);
 * if (RibbonCustomizeDialog::Accepted == dlg.exec()) {
 *    dlg.applys();
 *    QByteArray str;
 *    QXmlStreamWriter xml(&str);//QXmlStreamWriter不建议通过QString构造，遇到中文会异常
 *    xml.setAutoFormatting(true);
 *    xml.setAutoFormattingIndent(2);
 *    xml.setCodec("utf-8");//在writeStartDocument之前指定编码
 *    xml.writeStartDocument();
 *    bool isok = dlg.toXml(&xml);
 *    xml.writeEndDocument();
 *    if (isok) {
 *        QFile f("customize.xml");
 *        if (f.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate)) {
 *            QTextStream s(&f);
 *            s.setCodec("utf-8");//指定编码输出
 *            s << str;
 *            s.flush();
 *        }
 *        m_edit->append("write xml:");//m_edit的定义为：QTextEdit *m_edit;
 *        m_edit->append(str);
 *    }
 * }
 * @endcode
 * @return 如果出现异常，返回false,如果没有自定义数据也会返回false
 * @see sa_customize_datas_to_xml
 */
bool RibbonCustomizeWidget::toXml(QXmlStreamWriter *xml) const {
    QList<RibbonCustomizeData> res;

    res = m_d->mOldCustomizeDatas + m_d->mCustomizeDatas;
    res = RibbonCustomizeData::simplify(res);
    return (sa_customize_datas_to_xml(xml, res));
}


/**
 * @brief 把配置写入文件中
 * @param xmlpath
 * @return
 */
bool RibbonCustomizeWidget::toXml(const QString &xmlpath) const {
    QFile f(xmlpath);

    if (!f.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        return (false);
    }
    QXmlStreamWriter xml(&f);

    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(2);
    xml.setCodec("utf-8");//在writeStartDocument之前指定编码
    xml.writeStartDocument();
    bool isOK = toXml(&xml);

    xml.writeEndDocument();
    f.close();
    return (isOK);
}


/**
 * @brief 从xml中加载QList<RibbonCustomizeData>
 *
 * 对于基于配置文件的设置，对话框显示前建议调用此函数，保证叠加设置的正确记录
 * @param xml
 * @return
 * @note 此函数要在@ref setupActionsManager 函数之后调用
 */
void RibbonCustomizeWidget::fromXml(QXmlStreamReader *xml) {
    QList<RibbonCustomizeData> cds = sa_customize_datas_from_xml(xml, m_d->mActionMgr);

    m_d->mOldCustomizeDatas = cds;
}


/**
 * @brief 从xml中加载QList<RibbonCustomizeData>
 *
 * 对于基于配置文件的设置，对话框显示前建议调用此函数，保证叠加设置的正确记录
 * @param xmlpath
 * @note 此函数要在@ref setupActionsManager 函数之后调用
 * @note 如果程序启动后加载了自定义配置，再调用此窗口时需要调用此函数，把原来的配置加载进来，
 * 在生成新动作时会把旧动作保存，但在调用applys时不会调用此加载的动作
 */
void RibbonCustomizeWidget::fromXml(const QString &xmlpath) {
    QFile f(xmlpath);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    f.seek(0);
    QXmlStreamReader xml(&f);

    fromXml(&xml);
}


/**
 * @brief 应用xml配置
 *
 * @note 重复加载一个配置文件会发生异常，为了避免此类事件发生，一般通过一个变量保证只加载一次，如：
 * @code
 * //只能调用一次
 * static bool has_call = false;
 * if (!has_call) {
 *     QFile f("customize.xml");
 *     if (!f.open(QIODevice::ReadWrite|QIODevice::Text)) {
 *         return;
 *     }
 *     f.seek(0);
 *     QXmlStreamReader xml(&f);
 *     has_call = RibbonCustomizeWidget::fromXml(&xml, this, m_actMgr);
 * }
 * @endcode
 * @param xml
 * @param w
 * @return 所有设定有一个应用成功都会返回true
 * @see sa_customize_datas_from_xml sa_customize_datas_apply sa_apply_customize_from_xml_file
 */
bool RibbonCustomizeWidget::fromXml(QXmlStreamReader *xml, RibbonMainWindow *w, RibbonActionsManager *mgr) {
    //先找到sa-ribbon-customize标签
    QList<RibbonCustomizeData> cds = sa_customize_datas_from_xml(xml, mgr);

    return (sa_customize_datas_apply(cds, w) > 0);
}


/**
 * @brief 清除所有动作
 *
 * 在执行applys函数后，如果要继续调用，应该clear，否则会导致异常
 */
void RibbonCustomizeWidget::clear() {
    m_d->mCustomizeDatas.clear();
}


/**
 * @brief 精简
 */
void RibbonCustomizeWidget::simplify() {
    m_d->mCustomizeDatas = RibbonCustomizeData::simplify(m_d->mCustomizeDatas);
}


/**
 * @brief 获取当前界面选中的行属性
 * @return
 */
RibbonPannelItem::RowProportion RibbonCustomizeWidget::selectedRowProportion() const {
    return (static_cast<RibbonPannelItem::RowProportion>(ui->comboBoxActionProportion->currentData().toInt()));
}


/**
 * @brief 获取listview中选中的action
 * @return 如果没有选中action，返回nullptr
 * @note 如果要获取treeview选中的action，使用@ref itemToAction 函数
 */
QAction *RibbonCustomizeWidget::selectedAction() const {
    QItemSelectionModel *m = ui->listViewSelect->selectionModel();

    if ((nullptr == m) || !m->hasSelection()) {
        return (nullptr);
    }
    QModelIndex i = m->currentIndex();

    return (m_d->mAcionModel->indexToAction(i));
}


/**
 * @brief 把item转换为action
 * @param item
 * @return 如果没有action可转换，返回nullptr
 */
QAction *RibbonCustomizeWidget::itemToAction(QStandardItem *item) const {
    return (m_d->itemToAction(item));
}


/**
 * @brief 获取ribbon tree选中的item
 * @return
 */
QStandardItem *RibbonCustomizeWidget::selectedItem() const {
    QItemSelectionModel *m = ui->treeViewResult->selectionModel();

    if ((nullptr == m) || !m->hasSelection()) {
        return (nullptr);
    }
    QModelIndex i = m->currentIndex();

    return (m_d->mRibbonModel->itemFromIndex(i));
}


/**
 * @brief 获取选中的ribbon tree 的level
 * @return -1为选中异常，0代表选中了category 1代表选中了pannel 2代表选中了action
 */
int RibbonCustomizeWidget::selectedRibbonLevel() const {
    QStandardItem *item = selectedItem();

    if (item) {
        return (itemLevel(item));
    }
    return (-1);
}


/**
 * @brief 获取StandardItem 的level
 * @param item
 * @return
 */
int RibbonCustomizeWidget::itemLevel(QStandardItem *item) const {
    return (m_d->itemLevel(item));
}


/**
 * @brief 设置某个item被选中
 * @param item
 */
void RibbonCustomizeWidget::setSelectItem(QStandardItem *item, bool ensureVisible) {
    QItemSelectionModel *m = ui->treeViewResult->selectionModel();

    if (nullptr == m) {
        return;
    }
    if (m) {
        m->clearSelection();
        m->select(item->index(), QItemSelectionModel::Select);
    }
    if (ensureVisible) {
        ui->treeViewResult->scrollTo(item->index());
    }
}


/**
 * @brief 判断itemn能否改动，可以改动返回true
 * @param item
 * @return
 */
bool RibbonCustomizeWidget::isItemCanCustomize(QStandardItem *item) const {
    return (m_d->isItemCanCustomize(item));
}


bool RibbonCustomizeWidget::isSelectedItemCanCustomize() const {
    return (isItemCanCustomize(selectedItem()));
}


/**
 * @brief 判断itemn能否改动，可以改动返回true
 * @param item
 * @return
 */
bool RibbonCustomizeWidget::isCustomizeItem(QStandardItem *item) const {
    return (m_d->isCustomizeItem(item));
}


bool RibbonCustomizeWidget::isSelectedItemIsCustomize() const {
    return (isCustomizeItem(selectedItem()));
}


void RibbonCustomizeWidget::removeItem(QStandardItem *item) {
    if (item->parent()) {
        item->parent()->removeRow(item->row());
    } else {
        m_d->mRibbonModel->removeRow(item->row());
    }
}


void RibbonCustomizeWidget::onComboBoxActionIndexCurrentIndexChanged(int index) {
    int tag = ui->comboBoxActionIndex->itemData(index).toInt();

    m_d->mAcionModel->setFilter(tag);
}


void RibbonCustomizeWidget::onRadioButtonGroupButtonClicked(QAbstractButton *b) {
    updateModel(b == ui->radioButtonAllCategory ? ShowAllCategory : ShowMainCategory);
}


void RibbonCustomizeWidget::onPushButtonNewCategoryClicked() {
    int row = m_d->mRibbonModel->rowCount();
    QItemSelectionModel *m = ui->treeViewResult->selectionModel();

    if (m && m->hasSelection()) {
        QModelIndex i = m->currentIndex();
        while (i.parent().isValid()) {
            i = i.parent();
        }
        //获取选中的最顶层item
        row = i.row() + 1;
    }
    QStandardItem *ni = new QStandardItem(tr("new category[customize]%1").arg(++(m_d->mCustomizeCategoryCount)));

    ni->setData(0, RibbonCustomizeWidget::LevelRole);
    m_d->mRibbonModel->insertRow(row, ni);
    //设置新增的为选中
    setSelectItem(ni);
    //把动作插入动作列表中
    RibbonCustomizeData d = RibbonCustomizeData::makeAddCategoryCustomizeData(ni->text(), ni->row(),
                                                                              RibbonCustomizeWidgetPrivate::makeRandomObjName(
                                                                                      "category"));

    m_d->mCustomizeDatas.append(d);
    ni->setData(true, RibbonCustomizeWidget::CanCustomizeRole);//有CustomizeRole，必有CanCustomizeRole
    ni->setData(true, RibbonCustomizeWidget::CustomizeRole);
    ni->setData(d.categoryObjNameValue, RibbonCustomizeWidget::CustomizeObjNameRole);
}


void RibbonCustomizeWidget::onPushButtonNewPannelClicked() {
    QStandardItem *item = selectedItem();

    if (nullptr == item) {
        return;
    }
    int level = selectedRibbonLevel();

    QStandardItem *ni = new QStandardItem(tr("new pannel[customize]%1").arg(++(m_d->mCustomizePannelCount)));

    ni->setData(1, RibbonCustomizeWidget::LevelRole);

    if (0 == level) {
        //说明是category,插入到最后
        item->appendRow(ni);
    } else if (1 == level) {
        //说明选择的是pannel，插入到此pannel之后
        QStandardItem *categoryItem = item->parent();
        if (nullptr == categoryItem) {
            return;
        }
        categoryItem->insertRow(item->row() + 1, ni);
    } else {
        //不符合就删除退出
        delete ni;
        ni = nullptr;
        return;
    }
    //查找category的object name
    QStandardItem *categoryItem = ni->parent();
    QString categoryObjName = "";


    categoryObjName = m_d->itemObjectName(categoryItem);
    RibbonCustomizeData d = RibbonCustomizeData::makeAddPannelCustomizeData(ni->text(), ni->row(), categoryObjName,
                                                                            RibbonCustomizeWidgetPrivate::makeRandomObjName(
                                                                                    "pannel"));

    m_d->mCustomizeDatas.append(d);
    ni->setData(true, RibbonCustomizeWidget::CanCustomizeRole);//有CustomizeRole，必有CanCustomizeRole
    ni->setData(true, RibbonCustomizeWidget::CustomizeRole);
    ni->setData(d.pannelObjNameValue, RibbonCustomizeWidget::CustomizeObjNameRole);
    setSelectItem(ni);
}


void RibbonCustomizeWidget::onPushButtonRenameClicked() {
    QStandardItem *item = selectedItem();

    if (nullptr == item) {
        return;
    }
    bool ok;
    QString text = "";

    text = QInputDialog::getText(this, tr("rename"),
                                 tr("name:"), QLineEdit::Normal,
                                 item->text(), &ok);

    if (!ok || text.isEmpty()) {
        return;
    }
    int level = itemLevel(item);

    if (0 == level) {
        //改Category名
        QString cateObjName = m_d->itemObjectName(item);
        RibbonCustomizeData d = RibbonCustomizeData::makeRenameCategoryCustomizeData(text, cateObjName);
        m_d->mCustomizeDatas.append(d);
    } else if (1 == level) {
        QString cateObjName = m_d->itemObjectName(item->parent());
        QString pannelObjName = m_d->itemObjectName(item);
        RibbonCustomizeData d = RibbonCustomizeData::makeRenamePannelCustomizeData(text, cateObjName, pannelObjName);
        m_d->mCustomizeDatas.append(d);
    } else {
        //action 不允许改名
        return;
    }
    item->setText(text);
}


void RibbonCustomizeWidget::onPushButtonAddClicked() {
    QAction *act = selectedAction();
    QStandardItem *item = selectedItem();

    if ((nullptr == act) || (nullptr == item)) {
        return;
    }
    int level = itemLevel(item);

    if (0 == level) {
        //选中category不进行操作
        return;
    } else if (2 == level) {
        //选中action，添加到这个action之后,把item设置为pannel
        item = item->parent();
    }
    QString pannelObjName = m_d->itemObjectName(item);
    QString categoryObjName = m_d->itemObjectName(item->parent());
    QString key = m_d->mActionMgr->key(act);

    RibbonCustomizeData d = RibbonCustomizeData::makeAddActionCustomizeData(key, m_d->mActionMgr,
                                                                            selectedRowProportion(), categoryObjName,
                                                                            pannelObjName);

    m_d->mCustomizeDatas.append(d);

    QStandardItem *actItem = new QStandardItem(act->icon(), act->text());

    actItem->setData(2, RibbonCustomizeWidget::LevelRole);
    actItem->setData(true, RibbonCustomizeWidget::CanCustomizeRole);//有CustomizeRole，必有CanCustomizeRole
    actItem->setData(true, RibbonCustomizeWidget::CustomizeRole);
    actItem->setData(act->objectName(), RibbonCustomizeWidget::CustomizeObjNameRole);
    actItem->setData(qintptr(act), RibbonCustomizeWidget::PointerRole);//把action指针传入
    item->appendRow(actItem);
}


void RibbonCustomizeWidget::onPushButtonDeleteClicked() {
    QStandardItem *item = selectedItem();

    if (nullptr == item) {
        return;
    }
    if (!isItemCanCustomize(item)) {
        return;
    }
    int level = itemLevel(item);

    if (0 == level) {
        //删除category
        RibbonCustomizeData d = RibbonCustomizeData::makeRemoveCategoryCustomizeData(m_d->itemObjectName(item));
        m_d->mCustomizeDatas.append(d);
    } else if (1 == level) {
        //删除pannel
        QString catObjName = m_d->itemObjectName(item->parent());
        QString pannelObjName = m_d->itemObjectName(item);
        RibbonCustomizeData d = RibbonCustomizeData::makeRemovePannelCustomizeData(catObjName, pannelObjName);
        m_d->mCustomizeDatas.append(d);
    } else if (2 == level) {
        //删除Action
        QString catObjName = m_d->itemObjectName(item->parent()->parent());
        QString pannelObjName = m_d->itemObjectName(item->parent());
        QAction *act = itemToAction(item);
        QString key = m_d->mActionMgr->key(act);
        if (key.isEmpty() || catObjName.isEmpty() || pannelObjName.isEmpty()) {
            return;
        }

        RibbonCustomizeData d = RibbonCustomizeData::makeRemoveActionCustomizeData(catObjName, pannelObjName, key,
                                                                                   m_d->mActionMgr);
        m_d->mCustomizeDatas.append(d);
    }
    //执行删除操作
    removeItem(item);
    //删除后重新识别
    ui->pushButtonAdd->setEnabled(selectedAction() && isSelectedItemIsCustomize() && selectedRibbonLevel() > 0);
    ui->pushButtonDelete->setEnabled(isSelectedItemIsCustomize());
}


void RibbonCustomizeWidget::onListViewSelectClicked(const QModelIndex &index) {
    //每次点击，判断是否可以进行操作，决定pushButtonAdd和pushButtonDelete的显示状态
    //点击了listview，判断treeview的状态
    Q_UNUSED(index);
    ui->pushButtonAdd->setEnabled(isSelectedItemCanCustomize() && selectedRibbonLevel() > 0);
    ui->pushButtonDelete->setEnabled(isSelectedItemCanCustomize());
}


void RibbonCustomizeWidget::onTreeViewResultClicked(const QModelIndex &index) {
    Q_UNUSED(index);
    //每次点击，判断是否可以进行操作，决定pushButtonAdd和pushButtonDelete的显示状态
    QStandardItem *item = selectedItem();

    if (nullptr == item) {
        return;
    }
    int level = itemLevel(item);

    ui->pushButtonAdd->setEnabled(selectedAction() && (level > 0) && isItemCanCustomize(item));
    ui->pushButtonDelete->setEnabled(isItemCanCustomize(item));                     //有CustomizeRole，必有CanCustomizeRole
    ui->pushButtonRename->setEnabled(
            level != 2 || isItemCanCustomize(item));       //QAction 不能改名 ， 有CustomizeRole，必有CanCustomizeRole
}


void RibbonCustomizeWidget::onToolButtonUpClicked() {
    QStandardItem *item = selectedItem();

    if ((nullptr == item) || (0 == item->row())) {
        return;
    }
    int level = itemLevel(item);

    if (0 == level) {
        //移动category
        RibbonCustomizeData d = RibbonCustomizeData::makeChangeCategoryOrderCustomizeData(m_d->itemObjectName(item),
                                                                                          -1);
        m_d->mCustomizeDatas.append(d);
        int r = item->row();
        item = m_d->mRibbonModel->takeItem(r);
        m_d->mRibbonModel->removeRow(r);
        m_d->mRibbonModel->insertRow(r - 1, item);
    } else if (1 == level) {
        QStandardItem *paritem = item->parent();
        RibbonCustomizeData d = RibbonCustomizeData::makeChangePannelOrderCustomizeData(
                m_d->itemObjectName(paritem), m_d->itemObjectName(item), -1);
        m_d->mCustomizeDatas.append(d);
        int r = item->row();
        item = paritem->takeChild(r);
        paritem->removeRow(r);
        paritem->insertRow(r - 1, item);
    } else if (2 == level) {
        QStandardItem *pannelItem = item->parent();
        QStandardItem *categoryItem = pannelItem->parent();
        QAction *act = itemToAction(item);
        if (!act) {
            return;
        }
        QString key = m_d->mActionMgr->key(act);
        RibbonCustomizeData d = RibbonCustomizeData::makeChangeActionOrderCustomizeData(
                m_d->itemObjectName(categoryItem), m_d->itemObjectName(pannelItem), key, m_d->mActionMgr, -1);
        m_d->mCustomizeDatas.append(d);
        int r = item->row();
        item = pannelItem->takeChild(r);
        pannelItem->removeRow(r);
        pannelItem->insertRow(r - 1, item);
    }
}


void RibbonCustomizeWidget::onToolButtonDownClicked() {
    QStandardItem *item = selectedItem();

    if (item == nullptr) {
        return;
    }
    int count = 0;

    if (item->parent()) {
        count = item->parent()->rowCount();
    } else {
        count = m_d->mRibbonModel->rowCount();
    }
    if ((nullptr == item) || ((count - 1) == item->row())) {
        return;
    }
    int level = itemLevel(item);

    if (0 == level) {
        //移动category
        RibbonCustomizeData d = RibbonCustomizeData::makeChangeCategoryOrderCustomizeData(m_d->itemObjectName(item), 1);
        m_d->mCustomizeDatas.append(d);
        int r = item->row();
        item = m_d->mRibbonModel->takeItem(item->row());
        m_d->mRibbonModel->removeRow(r);
        m_d->mRibbonModel->insertRow(r + 1, item);
    } else if (1 == level) {
        QStandardItem *paritem = item->parent();
        RibbonCustomizeData d = RibbonCustomizeData::makeChangePannelOrderCustomizeData(
                m_d->itemObjectName(paritem), m_d->itemObjectName(item), 1);
        m_d->mCustomizeDatas.append(d);
        int r = item->row();
        item = paritem->takeChild(r);
        paritem->removeRow(r);
        paritem->insertRow(r + 1, item);
    } else if (2 == level) {
        QStandardItem *pannelItem = item->parent();
        QStandardItem *categoryItem = pannelItem->parent();
        QAction *act = itemToAction(item);
        if (!act) {
            return;
        }
        QString key = m_d->mActionMgr->key(act);
        RibbonCustomizeData d = RibbonCustomizeData::makeChangeActionOrderCustomizeData(
                m_d->itemObjectName(categoryItem), m_d->itemObjectName(pannelItem), key, m_d->mActionMgr, -1);
        m_d->mCustomizeDatas.append(d);
        int r = item->row();
        item = pannelItem->takeChild(r);
        pannelItem->removeRow(r);
        pannelItem->insertRow(r + 1, item);
    }
}


void RibbonCustomizeWidget::onItemChanged(QStandardItem *item) {
    if (item == nullptr) {
        return;
    }
    int level = itemLevel(item);

    if (0 == level) {
        if (item->isCheckable()) {
            QString objname = m_d->itemObjectName(item);
            RibbonCustomizeData d = RibbonCustomizeData::makeVisibleCategoryCustomizeData(objname, item->checkState() ==
                                                                                                   Qt::Checked);
            m_d->mCustomizeDatas.append(d);
        }
    }
}


void RibbonCustomizeWidget::onLineEditSearchActionTextEdited(const QString &text) {
    m_d->mAcionModel->search(text);
}
