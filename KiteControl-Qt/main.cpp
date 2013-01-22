#include "mainwindow.h"
#include <QApplication>
#include<opencv/cv.h>
#include<opencv/highgui.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
