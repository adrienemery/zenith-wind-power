#ifndef CONTROLALGORITHM_H
#define CONTROLALGORITHM_H

#include <QObject>
#include <QtCore>
#include <QVector>
#include <QVector2D>
#include <sstream>
#include "imageprocessing.h"
#include "kitecolortracker.h"
#include "quadrent.h"
#include "PID.h"


class ControlAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit ControlAlgorithm(QObject *parent = 0);
    std::string intToStdString(int number){

        std::stringstream ss;//create a stringstream
         ss << number;//add number to the stream
         return ss.str();//return a string with the contents of the stream

    }
    std::string floatToStdString(float number){

        std::stringstream ss;//create a stringstream
         ss << number;//add number to the stream
         return ss.str();//return a string with the contents of the stream

    }

    ~ControlAlgorithm();
    
signals:


    
public slots:

    void startPidTimer();

    void updatePID();

    void update();

    void setMinX(float x);

    void setMaxX(float x);

    void setMinY(float y);

    void setMaxY(float y);

    void drawToFrame(QVector2D kitePos, QVector2D heading);

    void kiteControlAlgorithm();

    void initGrid();

    ImageProcessing* getImageProcessingHandle(){

        return this->imageProcessingWindow;
    }

private:

    float calcAngleFromVectors(QVector2D vec1,QVector2D vec2);
    QVector2D kitePos;
    QVector2D kiteHeading;
    QVector2D leftPath;
    QVector2D rightPath;
    QVector2D leftEnd;
    QVector2D leftBegin;
    QVector2D rightEnd;
    QVector2D rightBegin;

    float minX;
    float maxX;
    float minY;
    float maxY;

    cv::Mat *currentFrame;
    KiteColorTracker *kiteColorTracker;
    ImageProcessing *imageProcessingWindow;
    // std::vector<QVector2D> kiteTracer;
    QVector<QVector2D> kiteTracer;

    //Quadrent stuff
    //control grid parameters
    Quadrant OUTER_GRID_BOUNDARY; //refer to: (direct link) https://dl.dropbox.com/u/28096936/kitePower/kiteControlQuadrants.jpg
    Quadrant Q1;
    Quadrant Q2;
    Quadrant Q3;
    Quadrant Q4;
    Quadrant Q5;

    int OUTER_GRID_OFFSET_X;
    int OUTER_GRID_OFFSET_Y;
    int POWER_ZONE_X;
    int POWER_ZONE_Y;
    int FRAME_HEIGHT;
    int FRAME_WIDTH;

    QVector2D AIMPOINT_QUAD_1;
    QVector2D AIMPOINT_QUAD_2;
    QVector2D AIMPOINT_QUAD_3;
    QVector2D AIMPOINT_QUAD_4;

    // PID stuff
    PID *pid;
    float Kp,Ki,Kd,interval;

    QTimer *pidTimer;

};

#endif // CONTROLALGORITHM_H
