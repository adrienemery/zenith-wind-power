#include "kitecolortracker.h"
#include <QtCore>
#include <opencv/cv.h>
#include <opencv/highgui.h>

KiteColorTracker::KiteColorTracker(QObject *parent) :
    QObject(parent)
{
    sampleRate = 25;

    // create timer object
    timer = new QTimer(this);
    // connect timer to update slot
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    // start timer
    timer->start(sampleRate);
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
}
