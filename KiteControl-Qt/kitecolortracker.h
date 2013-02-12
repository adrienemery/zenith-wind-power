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
    void writeToArduino(QString message);


public slots:
    void update();

    void setSampleRate(int msec);

    void setMinErrorX(int val);
    void setMinErrorY(int val);

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
    void adjustCamPosition(int,int);
    void toggleTrackKite();
    void setGainX(double);
    void setGainY(double);
    void setPanVal(int val);
    void setTiltVal(int val);

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
    int getMinErrorX(){return _minErrorX;}
    int getMinErrorY(){return _minErrorY;}

    bool isTracking();
    bool getRFIFlag();

    bool isPaused();

    void serialReady();

    void waitForSerial();

    bool loadFilterData(QString fileName);
    void save(QString fileName);

    void dataLogger();


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
    bool _serialReady;

    const static int FRAME_WIDTH = 640;
    const static int FRAME_HEIGHT = 480;

    const static int CAM_CENTER_Y = FRAME_HEIGHT/2;
    const static int CAM_CENTER_X = FRAME_WIDTH/2;

    int _minErrorX;
    int _minErrorY;
    int _panVal;
    int _tiltVal;
    double _gainX;
    double _gainY;

    bool _trackKite;
    bool _dataLoggerFileCreated;

    int _x,_y;

};

#endif // KITECOLORTRACKER_H
