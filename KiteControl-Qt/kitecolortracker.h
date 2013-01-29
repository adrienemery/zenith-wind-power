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
    //destructor
    
signals:

    void updated();

    
public slots:
    void update();

    void setSampleRate(int msec);

    void cleanup();


private:

    int sampleRate;
    //this is the location where the webcam is
    int src;
    //create frame to store cam stream in
    std::string winName;
    cv::Mat currentFrame;



};

#endif // KITECOLORTRACKER_H
