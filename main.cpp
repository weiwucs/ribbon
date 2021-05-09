#include <QApplication>
#include <QtGlobal>
#include "RibbonQuickAccessBar.h"
#include "mainwindow.h"

void log_out_put(QtMsgType type, const QMessageLogContext& context, const QString& msg){
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type){
        case QtDebugMsg:
            fprintf(stdout, "[Debug] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtInfoMsg:
            fprintf(stdout, "[Info] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;

        case QtWarningMsg:
            fprintf(stdout, "[Warning] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;

        case QtCriticalMsg:
            fprintf(stdout, "[Critical] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;

        case QtFatalMsg:
            fprintf(stdout, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            abort();
    }
#ifndef QT_NO_DEBUG_OUTPUT
    fflush(stdout);
#endif
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QElapsedTimer cost;
    qInstallMessageHandler(log_out_put);
    cost.start();
    mainwindow mw;
    qDebug() <<"window build cost:"<<cost.elapsed()<<" ms";
    mw.show();

    return app.exec();
}

