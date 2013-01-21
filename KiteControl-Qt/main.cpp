#include "mainwindow.h"
#include <QApplication>
#undef main

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //kyle is beyond great
    
    return a.exec();
}
