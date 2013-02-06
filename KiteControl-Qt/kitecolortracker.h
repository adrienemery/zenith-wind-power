#ifndef KITECOLORTRACKER_H
#define KITECOLORTRACKER_H

#include <QObject>
#include <QtCore>
#include <string>
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

    void beginCapture(std::string capType);
    void endCapture();



    QString intToString(int number){
        std::stringstream ss;//create a stringstream
        ss << number;//add number to the stream
        QString convert = QString::fromStdString(ss.str());
        return convert;//return a string with the contents of the stream
    }

signals:

    void dataUpdated();


public slots:
    void update();

    void setSampleRate(int msec);

    void setHSV(int,int,int,int,int,int);
    void setHmin(int);
    void setSmin(int);
    void setVmin(int);
    void setHmax(int);
    void setSmax(int);
    void setVmax(int);
    void setErodeSize(int);
    void setDilateSize(int);
    void setMaxArea(int);
    void setMinArea(int);
    void setEDflag(bool flag);
    void setRFIflag();
    void playPause();

    int getHmin();
    int getSmin();
    int getVmin();
    int getHmax();
    int getSmax();
    int getVmax();
    int getErodeSize();
    int getDilateSize();
    int getMaxArea();
    int getMinArea();

    bool isPaused();

    bool loadFilterData(QString fileName);
    void save(QString fileName);




    void filterKite(cv::Mat frame);

    void cleanup();


private:

    int sampleRate;
    //state of image processor
    std::string state;
    //this is the location where the webcam is
    int src;
    //create frame to store cam stream in
    std::string winName;
    std::string winName2;
    //matrices to store frames
    cv::Mat currentFrame,HSVframe,filteredFrame;


    //private HSV values
    int _Hmin,_Hmax,_Smin,_Smax,_Vmin,_Vmax;
    //the size of the morphological operators
    //basically how coarse or fine the filtering process will be.
    //bigger size = less definitive shape (we don't care about the shape)
    int _erodeSize,_dilateSize;
    int _minArea,_maxArea;

    bool _showDilateErode;
    bool _showRFI;
    bool _play;

};

#endif // KITECOLORTRACKER_H
