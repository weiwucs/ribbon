//
// Created by weiwu on 21-5-1.
//

#include "RibbonContextCategory.h"
#include "RibbonElementManager.h"


class RibbonCategoryData {
public:
    RibbonCategory *categoryPage;
};

class RibbonContextCategoryPrivate {
public:
    QList<RibbonCategoryData> categoryDataList;
    QVariant contextID;
    QColor contextColor;
    QString contextTitle;
};

RibbonContextCategory::RibbonContextCategory(QWidget *parent) : QObject(parent), m_d(new RibbonContextCategoryPrivate) {
}


RibbonContextCategory::~RibbonContextCategory() {
    delete m_d;
}


RibbonCategory *RibbonContextCategory::addCategoryPage(const QString &title) {
    RibbonCategoryData catData;
    RibbonCategory *category = RibbonSubElementDelegate->createRibbonCategory(parentWidget());

    category->markIsContextCategory(true);
    category->setWindowTitle(title);
    catData.categoryPage = category;
    m_d->categoryDataList.append(catData);
    category->installEventFilter(this);
    emit categoryPageAdded(category);

    return (category);
}


int RibbonContextCategory::categoryCount() const {
    return (m_d->categoryDataList.count());
}


void RibbonContextCategory::setId(const QVariant &id) {
    m_d->contextID = id;
}


QVariant RibbonContextCategory::id() const {
    return (m_d->contextID);
}


void RibbonContextCategory::setContextColor(const QColor color) {
    m_d->contextColor = color;
}


QColor RibbonContextCategory::contextColor() const {
    return (m_d->contextColor);
}


QWidget *RibbonContextCategory::parentWidget() const {
    return (qobject_cast<QWidget *>(parent()));
}


bool RibbonContextCategory::eventFilter(QObject *watched, QEvent *e) {
    if (nullptr == watched) {
        return (false);
    }
    switch (e->type()) {
        case QEvent::Close: {
            RibbonCategory *c = qobject_cast<RibbonCategory *>(watched);
            if (c) {
                takeCategory(c);
            }
        }
            break;

        default:
            break;
    }
    return (false);
}


/**
 * @brief 获取上下文标签的标题
 * @return
 */
QString RibbonContextCategory::contextTitle() const {
    return (m_d->contextTitle);
}


/**
 * @brief 设置上下文标签的标题，标题仅在office模式下显示 @ref RibbonBar::RibbonStyle
 * @param contextTitle
 */
void RibbonContextCategory::setContextTitle(const QString &contextTitle) {
    m_d->contextTitle = contextTitle;
}


/**
 * @brief 获取对应的tab页
 * @param index
 * @return
 */
RibbonCategory *RibbonContextCategory::categoryPage(int index) {
    return (m_d->categoryDataList[index].categoryPage);
}


/**
 * @brief 获取所有的RibbonCategory*
 * @return
 */
QList<RibbonCategory *> RibbonContextCategory::categoryList() const {
    QList<RibbonCategory *> res;

    for (RibbonCategoryData &c : m_d->categoryDataList) {
        res.append(c.categoryPage);
    }
    return (res);
}


/**
 * @brief 移除这个category，这时RibbonContextCategory不再管理这个category
 * @param category
 * @return 成功移除返回true
 */
bool RibbonContextCategory::takeCategory(RibbonCategory *category) {
    for (int i = 0; i < m_d->categoryDataList.size(); ++i) {
        if (m_d->categoryDataList[i].categoryPage == category) {
            m_d->categoryDataList.takeAt(i);
            return (true);
        }
    }
    return (false);
}


