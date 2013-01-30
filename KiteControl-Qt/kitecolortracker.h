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



public slots:
    void update();

    void setSampleRate(int msec);

    void setHSV(int,int,int,int,int,int);

    void filterKite(cv::Mat frame);

    void cleanup();


private:

    int sampleRate;
    //this is the location where the webcam is
    int src;
    //create frame to store cam stream in
    std::string winName;
    //matrices to store frames
    cv::Mat currentFrame,HSVframe,filteredFrame;


    //private HSV values
    int _Hmin,_Hmax,_Smin,_Smax,_Vmin,_Vmax;
    //the size of the morphological operators
    //basically how coarse or fine the filtering process will be.
    //bigger size = less definitive shape (we don't care about the shape)
    int _morphSize1,_morphSize2,_morphSize3;
    int _minArea,_maxArea;

};

#endif // KITECOLORTRACKER_H
