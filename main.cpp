#include <QApplication>
#include <QFormLayout>
#include <QtGlobal>
#include <QObject>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>
#include "FramelessHelper.h"
#include "RibbonGallery.h"
#include "RibbonQuickAccessBar.h"
#include "RibbonBar.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto window = new QWidget;
    window->setWindowFlags(Qt::FramelessWindowHint);

//    auto spinBox = new QSpinBox;
//    spinBox->setRange(0, 100);
//    auto slider = new QSlider;
//    slider->setRange(0, 100);
//    QObject::connect(spinBox,
//            // 对于重载函数需要给出确定的版本，QOverload需要编译器支持c++11
//                     QOverload<int>::of(&QSpinBox::valueChanged),
//                     slider,
//                     &QSlider::setValue);
//    QObject::connect(slider,
//                     &QSlider::valueChanged,
//                     spinBox,
//                     &QSpinBox::setValue);
//
    auto layout = new QHBoxLayout;

//    auto ribbonGallery = new RibbonGallery(window);
//    layout->addWidget(ribbonGallery);
//    auto ribbonQuickAccessBar = new RibbonQuickAccessBar(window);
//    ribbonQuickAccessBar->addAction(new QAction(QIcon("./icon/chartDataManager.png"), "action1", window));
//    ribbonQuickAccessBar->addAction(new QAction(QIcon("./icon/figureIcon.png"), "action2", window));
//    ribbonQuickAccessBar->addSeparator();
//    ribbonQuickAccessBar->addAction(new QAction(QIcon("./icon/information.png"), "action3", window));
//    layout->addWidget(ribbonQuickAccessBar);
//    layout->addRow("spin box:", spinBox);
//    layout->addRow("slider:", slider);

    RibbonBar *ribbonBar = new RibbonBar(window);
    ribbonBar->applicationButton()->setText("File");
    layout->addWidget(ribbonBar);

//    FramelessHelper *pHelper = new FramelessHelper(window);
//    pHelper->activateOn(window);  //激活当前窗体
//    pHelper->setTitleHeight(32);  //设置窗体的标题栏高度
//    pHelper->setWidgetMovable(true);  //设置窗体可移动
//    pHelper->setWidgetResizable(true);  //设置窗体可缩放
//    pHelper->setRubberBandOnMove(true);  //设置橡皮筋效果-可移动
//    pHelper->setRubberBandOnResize(true);  //设置橡皮筋效果-可缩放

    window->setLayout(layout);
    window->show();

    return app.exec();
}

