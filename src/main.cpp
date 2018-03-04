#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/Pic/startBack.jpg");
    QSplashScreen screen(pixmap);
    screen.show();
    screen.showMessage("数据报构造器", Qt::AlignCenter, Qt::red);

//    QDateTime n=QDateTime::currentDateTime();
//    QDateTime now;
//    do{
//        now=QDateTime::currentDateTime();
//    } while (n.secsTo(now)<=3);

    MainWindow w;
    w.show();

    screen.finish(&w);

    return a.exec();
}
