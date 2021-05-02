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
#include "RibbonMainWindow.h"
#include "mainwindow.h"

#define PRINT_COST(ElapsedTimer, LastTime, STR) \
    do{ \
        int ___TMP_INT = ElapsedTimer.elapsed(); \
        qDebug() << STR << ___TMP_INT - LastTime << "(" << ___TMP_INT << ")"; \
        LastTime = ___TMP_INT; \
    }while(0)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QElapsedTimer cost;
    int lastTimes = 0;
    cost.start();
    mainwindow mw;
    mw.show();

    return app.exec();
}

