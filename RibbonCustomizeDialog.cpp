//
// Created by weiwu on 21-5-2.
//

#include "RibbonCustomizeDialog.h"
#include <QPushButton>
#include <QApplication>
#include <QSpacerItem>

class RibbonCustomizeDialogUi {
public:
    RibbonCustomizeWidget *customWidget;
    QVBoxLayout *verticalLayoutMain;
    QHBoxLayout *horizontalLayoutButtonGroup;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonOk;
    QSpacerItem *spacerItemleft;

    void setupUi(RibbonMainWindow *ribbonWindow, QWidget *customizeDialog) {
        if (customizeDialog->objectName().isEmpty()) {
            customizeDialog->setObjectName(QStringLiteral("RibbonCustomizeDialog"));
        }
        customizeDialog->resize(800, 600);
        verticalLayoutMain = new QVBoxLayout(customizeDialog);
        verticalLayoutMain->setObjectName(QStringLiteral("verticalLayoutMain"));

        customWidget = new RibbonCustomizeWidget(ribbonWindow, customizeDialog);
        customWidget->setObjectName(QStringLiteral("customWidget"));
        verticalLayoutMain->addWidget(customWidget);

        horizontalLayoutButtonGroup = new QHBoxLayout();
        horizontalLayoutButtonGroup->setObjectName(QStringLiteral("horizontalLayoutButtonGroup"));

        spacerItemleft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        horizontalLayoutButtonGroup->addItem(spacerItemleft);

        pushButtonCancel = new QPushButton(customizeDialog);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        horizontalLayoutButtonGroup->addWidget(pushButtonCancel);

        pushButtonOk = new QPushButton(customizeDialog);
        pushButtonOk->setObjectName(QStringLiteral("pushButtonCancel"));
        horizontalLayoutButtonGroup->addWidget(pushButtonOk);
        verticalLayoutMain->addItem(horizontalLayoutButtonGroup);
        retranslateUi(customizeDialog);
    }


    void retranslateUi(QWidget *customizeDialog) {
        customizeDialog->setWindowTitle(
                QApplication::translate("RibbonCustomizeDialog", "Customize Dialog", Q_NULLPTR));
        pushButtonCancel->setText(QApplication::translate("RibbonCustomizeDialog", "Cancel", Q_NULLPTR));
        pushButtonOk->setText(QApplication::translate("RibbonCustomizeDialog", "OK", Q_NULLPTR));
    }
};


////////////////////////////////////////////////////////////////////

RibbonCustomizeDialog::RibbonCustomizeDialog(RibbonMainWindow *ribbonWindow, QWidget *p, Qt::WindowFlags f)
        : QDialog(p, f), ui(new RibbonCustomizeDialogUi) {
    ui->setupUi(ribbonWindow, this);
    initConnection();
}


/**
 * @brief ??????action?????????
 *
 * ??????@ref RibbonCustomizeWidget::setupActionsManager
 * @param mgr
 */
void RibbonCustomizeDialog::setupActionsManager(RibbonActionsManager *mgr) {
    ui->customWidget->setupActionsManager(mgr);
}


void RibbonCustomizeDialog::initConnection() {
    connect(ui->pushButtonOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
}


/**
 * @brief ??????RibbonCustomizeWidget::applys
 *
 * @ref RibbonCustomizeWidget::applys
 * @return
 */
bool RibbonCustomizeDialog::applys() {
    return (ui->customWidget->applys());
}


/**
 * @brief ??????????????????
 *
 * @ref RibbonCustomizeWidget::clear
 */
void RibbonCustomizeDialog::clear() {
    ui->customWidget->clear();
}


/**
 * @brief ?????????xml
 *
 * @ref RibbonCustomizeWidget::toXml
 * @param xml
 * @return
 */
bool RibbonCustomizeDialog::toXml(QXmlStreamWriter *xml) const {
    return (ui->customWidget->toXml(xml));
}


/**
 * @brief ??????RibbonCustomizeWidget::toXml
 * @ref RibbonCustomizeWidget::toXml
 * @param xmlpath
 * @return
 */
bool RibbonCustomizeDialog::toXml(const QString &xmlpath) const {
    return (ui->customWidget->toXml(xmlpath));
}


/**
 * @brief ??????RibbonCustomizeWidget::fromXml
 * @param xml
 */
void RibbonCustomizeDialog::fromXml(QXmlStreamReader *xml) {
    ui->customWidget->fromXml(xml);
}


/**
 * @brief ??????RibbonCustomizeWidget::fromXml
 * @param xmlpath
 */
void RibbonCustomizeDialog::fromXml(const QString &xmlpath) {
    ui->customWidget->fromXml(xmlpath);
}


/**
 * @brief ??????RibbonCustomizeWidget????????????
 *
 * ??????RibbonCustomizeWidget?????????????????????????????????
 *
 * @return RibbonCustomizeWidget???????????????@ref RibbonCustomizeWidget
 */
RibbonCustomizeWidget *RibbonCustomizeDialog::customizeWidget() const {
    return (ui->customWidget);
}
