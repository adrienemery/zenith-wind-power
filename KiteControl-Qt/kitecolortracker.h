#ifndef KITECOLORTRACKER_H
#define KITECOLORTRACKER_H

#include <QObject>
#include <QtCore>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "kite.h"

class KiteColorTracker : public QObject
{
    Q_OBJECT
public:
    explicit KiteColorTracker(QObject *parent = 0);

    QTimer *timer;

    Kite *kite;

    cv::VideoCapture *capture;



    int getSampleRate();
    
signals:


    
public slots:
    void update();

    void setSampleRate(int msec);



private:

    int sampleRate;
    //this is the location where the webcam is
    int src;
    std::string winName;
    cv::Mat currentFrame;



};

#endif // KITECOLORTRACKER_H
