#include "home.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    Home w;
    w.show();
    w.resize(640, 480);
    return a.exec();
}
