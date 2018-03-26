#include "mainwindow.h"
#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    MainWindow p;
    p.show();

    return a.exec();
}
