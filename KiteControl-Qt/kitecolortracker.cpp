#include "kitecolortracker.h"
#include <QtCore>


KiteColorTracker::KiteColorTracker(QObject *parent) :
    QObject(parent)
{
    sampleRate = 25;
    src = 0;
    winName = "camStream";

    // create timer object
    timer = new QTimer(this);
    // connect timer to update slot
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    // start timer
    timer->start(sampleRate);

    //start video capture
    capture = new cv::VideoCapture();
    capture->open(src);
    //create window to display capture
    cv::namedWindow(winName,1);
}

int KiteColorTracker::getSampleRate()
{
    return sampleRate;
}

void KiteColorTracker::setSampleRate(int msec)
{
    sampleRate = msec;
    // restart timer with new sample rate
    timer->start(sampleRate);
}

void KiteColorTracker::update()
{
    //qDebug() << sampleRate;

    //copy newest webcam image
    if(capture->isOpened()){

        capture->read(currentFrame);
      cv::imshow("camStream",currentFrame);

    }else qDebug()<<"error acquiring webcam stream";
}
