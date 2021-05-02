//
// Created by weiwu on 21-5-2.
//

#include "mainwindow.h"
#include "RibbonMenu.h"
#include "RibbonCustomizeDialog.h"
#include "RibbonComboBox.h"
#include "RibbonLineEdit.h"
#include "RibbonCheckBox.h"
#include "RibbonApplicationButton.h"

#define PRINT_COST(ElapsedTimer, LastTime, STR)                          \
    do{                                          \
        int ___TMP_INT = ElapsedTimer.elapsed();                  \
        qDebug() << STR << ___TMP_INT - LastTime << "(" << ___TMP_INT << ")"; \
        LastTime = ___TMP_INT;                              \
    }while(0)

mainwindow::mainwindow(QWidget *par) : RibbonMainWindow(par), m_customizeWidget(nullptr) {
    QElapsedTimer cost;
    int lastTimes = 0;

    cost.start();
    setWindowTitle(QStringLiteral("ribbon test"));
    m_edit = new QTextEdit(this);
    setCentralWidget(m_edit);
    PRINT_COST(cost, lastTimes, "setCentralWidget & setWindowTitle");
    RibbonBar *ribbon = ribbonBar();

    ribbon->applicationButton()->setText(QStringLiteral("File"));
    RibbonCategory *categoryMain = ribbon->addCategoryPage(QStringLiteral("Main"));

    PRINT_COST(cost, lastTimes, "new main page");
    createCategoryMain(categoryMain);
    PRINT_COST(cost, lastTimes, "add main page element");
    RibbonCategory *categoryOther = ribbon->addCategoryPage(QStringLiteral("Other"));

    createCategoryOther(categoryOther);
    PRINT_COST(cost, lastTimes, "add other page");
    m_contextCategory = ribbon->addContextCategory(QStringLiteral("context"), QColor(), 1);
    RibbonCategory *contextCategoryPage1 = m_contextCategory->addCategoryPage(QStringLiteral("Page1"));

    createContextCategoryPage1(contextCategoryPage1);
    RibbonCategory *contextCategoryPage2 = m_contextCategory->addCategoryPage(QStringLiteral("Page1"));

    createContextCategoryPage2(contextCategoryPage2);
    PRINT_COST(cost, lastTimes, "add contextCategory page");
    RibbonQuickAccessBar *quickAccessBar = ribbon->quickAccessBar();

    quickAccessBar->addAction(new QAction(QIcon("./icon/chartDataManager.png"), "action1", this));
    quickAccessBar->addAction(new QAction(QIcon("./icon/figureIcon.png"), "action2", this));
    quickAccessBar->addSeparator();
    quickAccessBar->addAction(new QAction(QIcon("./icon/information.png"), "action3", this));
    QMenu *m = new QMenu("action menu", this);

    m->setIcon(QIcon("./icon/inRangDataRemove.png"));
    m->addAction("1");
    m->addAction("2");
    m->addAction("3");
    m->addAction("4");
    m->addAction("5");
    quickAccessBar->addMenu(m);

    addSomeOtherAction();
    QAction *customize = new QAction(QIcon("./icon/chartDataManager.png"), "customize", this);

    quickAccessBar->addAction(customize);
    m_actMgr->autoRegisteActions(this);
    connect(customize, &QAction::triggered, this, [&]() {
        if (nullptr == m_customizeWidget) {
            m_customizeWidget = new RibbonCustomizeWidget(this, this,
                                                          Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint |
                                                          Qt::Dialog);
            m_customizeWidget->setWindowModality(Qt::ApplicationModal);     //设置阻塞类型
            m_customizeWidget->setAttribute(Qt::WA_ShowModal, true);        //属性设置 true:模态 false:非模态
            m_customizeWidget->setupActionsManager(m_actMgr);
        }
        m_customizeWidget->show();
        m_customizeWidget->applys();
    });

    QAction *customize2 = new QAction(QIcon("./icon/chartDataManager.png"), "customize2", this);

    quickAccessBar->addAction(customize2);
    connect(customize2, &QAction::triggered, this, [&]() {
        RibbonCustomizeDialog dlg(this);
        dlg.setupActionsManager(m_actMgr);
        dlg.fromXml("customize.xml");
        if (RibbonCustomizeDialog::Accepted == dlg.exec()) {
            dlg.applys();
            QByteArray str;
            QXmlStreamWriter xml(&str);
            xml.setAutoFormatting(true);
            xml.setAutoFormattingIndent(2);
            xml.setCodec("utf-8");
            xml.writeStartDocument();
            bool isok = dlg.toXml(&xml);
            xml.writeEndDocument();
            if (isok) {
                QFile f("customize.xml");
                if (f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
                    QTextStream s(&f);
                    s.setCodec("utf-8");
                    s << str;
                    s.flush();
                }
                m_edit->append("write xml:");
                m_edit->append(str);
            }
        }
    });
    //
    showMaximized();
}


void mainwindow::onShowContextCategory(bool on) {
    ribbonBar()->setContextCategoryVisible(m_contextCategory, on);
}


void mainwindow::onStyleClicked(int id) {
    ribbonBar()->setRibbonStyle(static_cast<RibbonBar::RibbonStyle>(id));
}


void mainwindow::createCategoryMain(RibbonCategory *page) {
    QElapsedTimer cost;

    cost.start();
    RibbonToolButton *btn;
    RibbonMenu *menu = new RibbonMenu(this);
    QAction *a = nullptr;

    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 1"));
    a->setObjectName(QStringLiteral("item 1"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 2"));
    a->setObjectName(QStringLiteral("item 2"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 3"));
    a->setObjectName(QStringLiteral("item 3"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 4"));
    a->setObjectName(QStringLiteral("item 4"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 5"));
    a->setObjectName(QStringLiteral("item 5"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 6"));
    a->setObjectName(QStringLiteral("item 6"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 7"));
    a->setObjectName(QStringLiteral("item 7"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 8"));
    a->setObjectName(QStringLiteral("item 8"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 9"));
    a->setObjectName(QStringLiteral("item 9"));
    a = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("item 10"));
    a->setObjectName(QStringLiteral("item 10"));

    int lastTimes = 0;
    RibbonPannel *pannel = page->addPannel(QStringLiteral("Panel 1"));

    PRINT_COST(cost, lastTimes, "addPannel pannel 1");

    QAction *act = new QAction(this);

    act->setObjectName(QStringLiteral("Save"));
    act->setIcon(QIcon("./icon/save.png"));
    act->setText(QStringLiteral("Save"));
    act->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    PRINT_COST(cost, lastTimes, "new Large Action");
    pannel->addLargeAction(act);
    connect(act, &QAction::triggered, this, [this](bool b) {
        this->m_edit->append("action clicked");
    });
    PRINT_COST(cost, lastTimes, "add 1 Large Action");

    act = new QAction(this);
    act->setObjectName(QStringLiteral("hide ribbon"));
    act->setIcon(QIcon("./icon/save.png"));
    act->setText(QStringLiteral("hide ribbon"));
    act->setCheckable(true);
    pannel->addSmallAction(act);
    connect(act, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->setMinimumMode(b);
    });

    act = new QAction(this);
    act->setObjectName(QStringLiteral("show hide button"));
    act->setIcon(QIcon("./icon/save.png"));
    act->setText(QStringLiteral("show hide button"));
    act->setCheckable(true);
    pannel->addSmallAction(act);
    connect(act, &QAction::triggered, this, [this](bool b) {
        this->ribbonBar()->showMinimumModeButton(b);
    });
    QButtonGroup *g = new QButtonGroup(page);
    QRadioButton *r = new QRadioButton();

    r->setText(QStringLiteral("use office style"));
    r->setObjectName(QStringLiteral("use office style"));
    r->setChecked(true);
    pannel->addSmallWidget(r);
    g->addButton(r, RibbonBar::OfficeStyle);
    r = new QRadioButton();
    r->setObjectName(QStringLiteral("use wps style"));
    r->setText(QStringLiteral("use wps style"));
    r->setChecked(false);
    pannel->addSmallWidget(r);
    g->addButton(r, RibbonBar::WpsLiteStyle);
    r = new QRadioButton();
    r->setObjectName(QStringLiteral("use office 2row style"));
    r->setText(QStringLiteral("use office 2row style"));
    r->setChecked(false);
    pannel->addSmallWidget(r);
    g->addButton(r, RibbonBar::OfficeStyleTwoRow);
    r = new QRadioButton();
    r->setObjectName(QStringLiteral("use wps 2row style"));
    r->setText(QStringLiteral("use wps 2row style"));
    r->setChecked(false);
    pannel->addSmallWidget(r);
    g->addButton(r, RibbonBar::WpsLiteStyleTwoRow);

//    connect(g, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &mainwindow::onStyleClicked);
    connect(g, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this,
            &mainwindow::onStyleClicked);
    act = new QAction(this);
    act->setObjectName(QStringLiteral("test 1"));
    act->setIcon(QIcon("./icon/folder.png"));
    act->setText(QStringLiteral("test 1"));
    act->setMenu(menu);
    btn = pannel->addSmallAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);

    act = new QAction(this);
    act->setObjectName(QStringLiteral("test 2"));
    act->setIcon(QIcon("./icon/folder.png"));
    act->setText(QStringLiteral("test 2"));
    act->setMenu(menu);
    btn = pannel->addSmallAction(act);
    btn->setPopupMode(QToolButton::InstantPopup);

    pannel->addSeparator();


    act = new QAction(this);
    act->setObjectName(QStringLiteral("DelayedPopup"));
    act->setIcon(QIcon("./icon/folder.png"));
    act->setText(QStringLiteral("DelayedPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setObjectName("_DelayedPopup");
    connect(act, &QAction::triggered, this, &mainwindow::onDelayedPopupCheckabletriggered);

    act = new QAction(this);
    act->setIcon(QIcon("./icon/folder.png"));
    act->setText(QStringLiteral("MenuButtonPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setObjectName(QStringLiteral("MenuButtonPopup"));
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    connect(act, &QAction::triggered, this, &mainwindow::onMenuButtonPopupCheckabletriggered);

    act = new QAction(this);
    act->setIcon(QIcon("./icon/Graph-add.png"));
    act->setText(QStringLiteral("InstantPopup"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setObjectName("buttonInstantPopup");
    btn->setPopupMode(QToolButton::InstantPopup);
    connect(act, &QAction::triggered, this, &mainwindow::onInstantPopupCheckabletriggered);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon("./icon/folder.png"));
    act->setText(QStringLiteral("DelayedPopup checkable"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setCheckable(true);
    btn->setObjectName("DelayedPopup checkable");
    connect(act, &QAction::triggered, this, &mainwindow::onDelayedPopupCheckableTest);

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon("./icon/folder.png"));
    act->setText(QStringLiteral("MenuButtonPopup checkable"));
    act->setMenu(menu);
    btn = pannel->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    btn->setCheckable(true);
    connect(act, &QAction::triggered, this, &mainwindow::onMenuButtonPopupCheckableTest);


    RibbonPannel *panne2 = page->addPannel(QStringLiteral("pannel 2"));

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon("./icon/Graph-add.png"));
    act->setText(QStringLiteral("show Context"));
    btn = panne2->addLargeAction(act);
    btn->setCheckable(true);
    //btn->setPopupMode(QToolButton::InstantPopup);
    connect(act, &QAction::triggered, this, [this](bool on) {
        if (on) {
            this->ribbonBar()->showContextCategory(this->m_contextCategory);
        } else {
            this->ribbonBar()->hideContextCategory(this->m_contextCategory);
        }
    });

    QAction *act2 = new QAction(this);

    act2->setIcon(QIcon("./icon/529398.png"));
    act2->setText(QStringLiteral("delete Context"));
    btn = panne2->addLargeAction(act2);
    connect(act2, &QAction::triggered, this, [this, act](bool on) {
        if (this->m_contextCategory) {
            this->ribbonBar()->destroyContextCategory(this->m_contextCategory);
            this->m_contextCategory = nullptr;
            act->setDisabled(true);
        }
    });

    act = new QAction(this);
    act->setCheckable(true);
    act->setIcon(QIcon("./icon/Graph-add.png"));
    act->setText(QStringLiteral("unactive"));
    act->setMenu(menu);
    btn = panne2->addLargeAction(act);
    btn->setCheckable(true);
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setEnabled(false);


    RibbonComboBox *com = new RibbonComboBox(this);

    com->setSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Fixed);
    com->setWindowIcon(QIcon("./icon/Graph-add.png"));
    com->setWindowTitle("ComboBox");
    for (int i = 0; i < 40; ++i) {
        com->addItem(QString("RibbonComboBox test%1").arg(i + 1));
    }
    com->setEditable(true);
    pannel->addSmallWidget(com);

    com = new RibbonComboBox(this);
    com->setSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Fixed);
    com->setWindowIcon(QIcon("./icon/folder.png"));
    com->setWindowTitle("ComboBox Editable");
    for (int i = 0; i < 40; ++i) {
        com->addItem(QString("item %1").arg(i + 1));
    }
    pannel->addSmallWidget(com);

    RibbonLineEdit *lineEdit = new RibbonLineEdit(this);

    lineEdit->setSizePolicy(QSizePolicy::Expanding,
                            QSizePolicy::Fixed);
    lineEdit->setWindowIcon(QIcon("./icon/folder.png"));
    lineEdit->setWindowTitle("Line Edit");
    lineEdit->setText("RibbonLineEdit");
    pannel->addSmallWidget(lineEdit);
    QWidget *w = lineEdit->parentWidget();

    while (w) {
        qDebug() << w->metaObject()->className();
        w = w->parentWidget();
    }
    RibbonCheckBox *checkBox = new RibbonCheckBox(this);

    checkBox->setWindowIcon(QIcon("./icon/folder.png"));
    checkBox->setText("checkBox");
    pannel->addSmallWidget(checkBox);
    pannel->addSeparator();
    QCalendarWidget *calendarWidget = new QCalendarWidget(this);

    calendarWidget->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);
    calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
    pannel->addLargeWidget(calendarWidget);
    pannel->setExpanding();
    QAction *optAct = new QAction(this);

    pannel->addOptionAction(optAct);

    pannel->setVisible(true);
}


void mainwindow::createCategoryOther(RibbonCategory *page) {
    RibbonMenu *menu = new RibbonMenu(this);
    QAction *item = menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("menu item test"));

    item->setObjectName(QStringLiteral("menu item test"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("1"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("2"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("3"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("4"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("5"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("6"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("7"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("8"));
    menu->addAction(QIcon("./icon/folder.png"), QStringLiteral("9"));

    RibbonPannel *pannel = page->addPannel(QStringLiteral("pannel one"));
    RibbonButtonGroupWidget *btnGroup = new RibbonButtonGroupWidget(pannel);

    btnGroup->setFrameShape(QFrame::Box);
    btnGroup->addAction(new QAction(QIcon("./icon/figureIcon.png"), "", this));
    btnGroup->addAction(new QAction(QIcon("./icon/information.png"), "", this));
    pannel->addLargeWidget(btnGroup);

    RibbonToolButton *btn;

    btn = pannel->addLargeAction(item);
    btn->setIcon(QIcon("./icon/folder.png"));
    btn->setText(QStringLiteral("un format icon"));
    btn->setPopupMode(QToolButton::DelayedPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addLargeWidget(btn);

    btn = pannel->addLargeAction(item);
    btn->setIcon(QIcon("./icon/folder.png"));
    btn->setText(QStringLiteral("change page test"));
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addLargeWidget(btn);

    btn = pannel->addLargeAction(item);
    btn->setIcon(QIcon("./icon/folder.png"));
    btn->setText(QStringLiteral("LargeBtn"));
    btn->setPopupMode(QToolButton::InstantPopup);
    btn->setFixedHeight(78);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setMenu(menu);
    pannel->addLargeWidget(btn);

    RibbonGallery *gallery = pannel->addGallery();
    RibbonGalleryGroup *group = gallery->addGalleryGroup();

    for (int i = 0; i < 100; ++i) {
        group->addItem(QIcon("./icon/folder.png"));
    }
    QAction *optAct = new QAction(this);

    optAct->setObjectName(QStringLiteral("debug"));
    pannel->addOptionAction(optAct);
    pannel->setObjectName("debug");

    pannel->addSeparator();
    QAction *appBtn = new QAction(QIcon("./icon/icon2.png"), tr("no application button and very long word test"),
                                  this);

    appBtn->setObjectName(QStringLiteral("no application button and very long word test"));
    appBtn->setCheckable(true);
    connect(appBtn, &QAction::toggled, this, [&](bool b) {
        if (b) {
            this->ribbonBar()->setApplicationButton(nullptr);
        } else {
            this->ribbonBar()->setApplicationButton(new RibbonApplicationButton());
        }
    });
    RibbonToolButton *b = pannel->addLargeAction(appBtn);

    b->setObjectName("ApplicationButtonTest");

    QAction *useqss = new QAction(QIcon("./icon/icon2.png"), tr("use qss"), this);

    useqss->setObjectName(QStringLiteral("use qss"));
    pannel->addLargeAction(useqss);
    connect(useqss, &QAction::triggered, this, [&]() {
        QFile f("ribbon.qss");
        if (!f.open(QIODevice::ReadWrite)) {
            return;
        }
        QString qss(f.readAll());
        this->ribbonBar()->setStyleSheet(qss);
    });

    QAction *useCustomize = new QAction(QIcon("./icon/506407.png"), tr("use customize from xml file"), this);

    useCustomize->setObjectName(QStringLiteral("useCustomize"));
    pannel->addLargeAction(useCustomize);
    connect(useCustomize, &QAction::triggered, this, [&]() {
        //只能调用一次
        static bool has_call = false;
        if (!has_call) {
            has_call = sa_apply_customize_from_xml_file("customize.xml", this, m_actMgr);
        }
    });
}


void mainwindow::createContextCategoryPage1(RibbonCategory *page) {
    RibbonPannel *pannel = page->addPannel(QStringLiteral("显示隐藏操作"));
    QAction *act = new QAction(this);

    act->setCheckable(true);
    act->setIcon(QIcon("./icon/530150.png"));
    act->setText(QStringLiteral("隐藏pannel"));
    pannel->addLargeAction(act);

    QAction *act2 = new QAction(this);

    act2->setDisabled(true);
    act2->setIcon(QIcon("./icon/529398.png"));
    act2->setText(QStringLiteral("disable"));
    pannel->addLargeAction(act2);
    connect(act2, &QAction::triggered, this, [act2](bool b) {
        act2->setDisabled(true);
    });

    QAction *act21 = new QAction(this);

    act21->setIcon(QIcon("./icon/529398.png"));
    act21->setText(QStringLiteral("解锁左边的按钮"));
    act21->setShortcut(QKeySequence(QLatin1String("Ctrl+E")));
    pannel->addLargeAction(act21);
    connect(act21, &QAction::triggered, this, [act2](bool b) {
        qDebug() << "act2->setEnabled(true);";
        act2->setEnabled(true);
        act2->setText(QStringLiteral("Enabled"));
    });

    QAction *act3 = new QAction(this);

    act3->setCheckable(true);
    act3->setIcon(QIcon("./icon/530767.png"));
    act3->setText(QStringLiteral("setText测试\r\nCtrl+D"));
    act3->setShortcut(QKeySequence(QLatin1String("Ctrl+D")));
    pannel->addLargeAction(act3);

    connect(act3, &QAction::toggled, this, [act3](bool b) {
        if (b) {
            act3->setText(QStringLiteral("点击了"));
        } else {
            act3->setText(QStringLiteral("setText测试"));
        }
    });
    //隐藏pannel
    QAction *act4 = new QAction(this);

    act4->setCheckable(true);
    act4->setIcon(QIcon("./icon/arror.png"));
    act4->setText(QStringLiteral("显示旁边的pannel"));
    pannel->addLargeAction(act4);

    RibbonPannel *pannel2 = page->addPannel(QStringLiteral("用于隐藏显示的测试"));

    pannel2->addLargeAction(act3);

    connect(act4, &QAction::toggled, this, [act4, pannel2, this](bool b) {
        pannel2->setVisible(!b);
        if (b) {
            act4->setText(QStringLiteral("隐藏旁边的pannel"));
        } else {
            act4->setText(QStringLiteral("显示旁边的pannel"));
        }
        ribbonBar()->repaint();
    });

    RibbonPannel *pannel3 = page->addPannel(QStringLiteral("action隐藏显示的测试"));
    QAction *act31 = new QAction(this);

    act31->setCheckable(true);
    act31->setChecked(true);
    act31->setIcon(QIcon("./icon/arror.png"));
    act31->setText(QStringLiteral("隐藏action2"));
    QAction *act32 = new QAction(this);

    act32->setIcon(QIcon("./icon/arror.png"));
    act32->setText(QStringLiteral("action 2"));
    QAction *act33 = new QAction(this);

    act33->setIcon(QIcon("./icon/arror.png"));
    act33->setText(QStringLiteral("action 3"));
    QAction *act34 = new QAction(this);

    act34->setIcon(QIcon("./icon/arror.png"));
    act34->setText(QStringLiteral("action 4"));
    pannel3->addLargeAction(act31);
    pannel3->addSmallAction(act32);
    pannel3->addSmallAction(act33);
    pannel3->addSmallAction(act34);
    connect(act31, &QAction::triggered, this, [act31, act32](bool b) {
        if (b) {
            act32->setVisible(true);
            act31->setText(QStringLiteral("隐藏action2"));
        } else {
            act32->setVisible(false);
            act31->setText(QStringLiteral("显示action2"));
        }
    });
}


void mainwindow::createContextCategoryPage2(RibbonCategory *page) {
    RibbonPannel *pannel1 = page->addPannel(QStringLiteral("删除CategoryPage测试"));
    QAction *act11 = new QAction(this);

    act11->setIcon(QIcon("./icon/529398.png"));
    act11->setText(QStringLiteral("删除本页"));
    pannel1->addLargeAction(act11);
    connect(act11, &QAction::triggered, this, [this, page]() {
        this->ribbonBar()->removeCategory(page);
        page->deleteLater();
    });
    RibbonPannel *pannel2 = page->addPannel(QStringLiteral("特殊布局"));

    pannel2->addAction(QStringLiteral("Large"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Large);
    pannel2->addAction(QStringLiteral("Small"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addAction(QStringLiteral("Small"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addSeparator();
    pannel2->addAction(QStringLiteral("Small"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addAction(QStringLiteral("Small"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addAction(QStringLiteral("Small"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addAction(QStringLiteral("Small"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addSeparator();
    pannel2->addAction(QStringLiteral("Large"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Large);
    pannel2->addAction(QStringLiteral("Medium"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Medium);
    pannel2->addAction(QStringLiteral("Medium"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Medium);
    pannel2->addAction(QStringLiteral("Large"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addAction(QStringLiteral("Large"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addAction(QStringLiteral("Large"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
    pannel2->addAction(QStringLiteral("Medium"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Medium);
    pannel2->addAction(QStringLiteral("Medium"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Medium);
    pannel2->addAction(QStringLiteral("Large"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Large);
    pannel2->addSeparator();
    pannel2->addAction(QStringLiteral("Medium"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Medium);
    pannel2->addAction(QStringLiteral("Large"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Large);
    pannel2->addAction(QStringLiteral("Medium"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Medium);
    pannel2->addAction(QStringLiteral("Small"), QIcon("./icon/530767.png"), QToolButton::InstantPopup,
                       RibbonPannelItem::Small);
}


void mainwindow::addSomeOtherAction() {
    m_actMgr = new RibbonActionsManager(this);
    //添加其他的action，这些action并不在ribbon管理范围，主要用于RibbonCustomizeWidget自定义用
    QAction *acttext1 = new QAction(QStringLiteral("纯文本action1"));
    QAction *acttext2 = new QAction(QStringLiteral("纯文本action2"));
    QAction *acttext3 = new QAction(QStringLiteral("纯文本action3"));
    QAction *acttext4 = new QAction(QStringLiteral("纯文本action4"));
    QAction *acttext5 = new QAction(QStringLiteral("纯文本action5"));

    QAction *actIcon1 = new QAction(QIcon("./icon/506353.png"), QStringLiteral("带图标action1"));
    QAction *actIcon2 = new QAction(QIcon("./icon/506354.png"), QStringLiteral("带图标action2"));
    QAction *actIcon3 = new QAction(QIcon("./icon/506355.png"), QStringLiteral("带图标action3"));
    QAction *actIcon4 = new QAction(QIcon("./icon/506356.png"), QStringLiteral("带图标action4"));

    m_actionTagText = RibbonActionsManager::UserDefineActionTag + 1;
    m_actionTagWithIcon = RibbonActionsManager::UserDefineActionTag + 2;
    m_actMgr->registeAction(acttext1, RibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(acttext3, RibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(acttext5, RibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(actIcon1, RibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(actIcon3, RibbonActionsManager::CommonlyUsedActionTag);

    m_actMgr->registeAction(acttext1, m_actionTagText);
    m_actMgr->registeAction(acttext2, m_actionTagText);
    m_actMgr->registeAction(acttext3, m_actionTagText);
    m_actMgr->registeAction(acttext4, m_actionTagText);
    m_actMgr->registeAction(acttext5, m_actionTagText);

    m_actMgr->registeAction(actIcon1, m_actionTagWithIcon);
    m_actMgr->registeAction(actIcon2, m_actionTagWithIcon);
    m_actMgr->registeAction(actIcon3, m_actionTagWithIcon);
    m_actMgr->registeAction(actIcon4, m_actionTagWithIcon);

    m_actMgr->setTagName(RibbonActionsManager::CommonlyUsedActionTag, QStringLiteral("常用"));
    m_actMgr->setTagName(m_actionTagText, QStringLiteral("无图标action"));
    m_actMgr->setTagName(m_actionTagWithIcon, QStringLiteral("有图标action"));
}


void mainwindow::onMenuButtonPopupCheckableTest(bool b) {
    m_edit->append(QStringLiteral("MenuButtonPopupCheckableTest : %1").arg(b));
}


void mainwindow::onInstantPopupCheckableTest(bool b) {
    m_edit->append(QStringLiteral("InstantPopupCheckableTest : %1").arg(b));
}


void mainwindow::onDelayedPopupCheckableTest(bool b) {
    m_edit->append(QStringLiteral("DelayedPopupCheckableTest : %1").arg(b));
}


void mainwindow::onMenuButtonPopupCheckabletriggered(bool b) {
    m_edit->append(QStringLiteral("MenuButtonPopupCheckabletriggered : %1").arg(b));
}


void mainwindow::onInstantPopupCheckabletriggered(bool b) {
    m_edit->append(QStringLiteral("InstantPopupCheckabletriggered : %1").arg(b));
}


void mainwindow::onDelayedPopupCheckabletriggered(bool b) {
    m_edit->append(QStringLiteral("DelayedPopupCheckabletriggered : %1").arg(b));
}
