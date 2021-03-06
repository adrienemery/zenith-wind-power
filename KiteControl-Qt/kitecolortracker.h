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

    int BOUNDING_RADIUS;

    QString intToString(int number){
        std::stringstream ss;//create a stringstream
        ss << number;//add number to the stream
        QString convert = QString::fromStdString(ss.str());
        return convert;//return a string with the contents of the stream
    }
   int FRAME_WIDTH;
   int FRAME_HEIGHT;

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
    void setCenterPan(int panVal);
    void setCenterTilt(int tiltVal);

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
    int getPanVal() {return _panVal;}
    int getTiltVal() {return _tiltVal;}


    //void createNewDataLog();

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

    void updateKiteData(int px,int py);

    cv::Mat* getFrameHandle(){
        return this->frameHandle;
    }

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
    cv::Mat *currentFrame,HSVframe,filteredFrame;

    cv::Mat *frameHandle;

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



    int CAM_CENTER_Y;
    int CAM_CENTER_X;

    int _minErrorX;
    int _minErrorY;
    int _panVal;
    int _tiltVal;
    double _gainX;
    double _gainY;

    bool _trackKite;
    bool _dataLoggerFileCreated;

    static const int _movingLeft = 0;
    static const int _movingRight = 1;
    static const int _movingUp = 2;
    static const int _movingDown = 3;

    QList<int> averageX;
    QList<int> averageY;
    int count;

    int xState;
    int yState;
    int lastXState;
    int lastYState;

    int lastPanVal;
    int lastTiltVal;

    int _x,_y;
    int _lastX, _lastY;
    int _centerPan, _centerTilt;



    //iterator for saving files with 1,2,3,4....such as to not overwrite
    int i;

    QString dataLogPath;

};

#endif // KITECOLORTRACKER_H
