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
    Quadrant getQ1(){return this->Q1;}
    Quadrant getQ2(){return this->Q2;}
    Quadrant getQ3(){return this->Q3;}
    Quadrant getQ4(){return this->Q4;}
    Quadrant getQ5(){return this->Q5;}
    Quadrant getOGB(){return this->OUTER_GRID_BOUNDARY;}
    QVector2D getAP1(){return this->AIMPOINT_QUAD_1;}
    QVector2D getAP2(){return this->AIMPOINT_QUAD_2;}
    QVector2D getAP3(){return this->AIMPOINT_QUAD_3;}
    QVector2D getAP4(){return this->AIMPOINT_QUAD_4;}
    void setQ1(int lx, int rx,int ty,int by){
        this->Q1.setLeftX(lx);
        this->Q1.setRightX(rx);
        this->Q1.setTopY(ty);
        this->Q1.setBottomY(by);
    }
    void setQ2(int lx, int rx,int ty,int by){
        this->Q2.setLeftX(lx);
        this->Q2.setRightX(rx);
        this->Q2.setTopY(ty);
        this->Q2.setBottomY(by);
    }
    void setQ3(int lx, int rx,int ty,int by){
        this->Q3.setLeftX(lx);
        this->Q3.setRightX(rx);
        this->Q3.setTopY(ty);
        this->Q3.setBottomY(by);
    }
    void setQ4(int lx, int rx,int ty,int by){
        this->Q4.setLeftX(lx);
        this->Q4.setRightX(rx);
        this->Q4.setTopY(ty);
        this->Q4.setBottomY(by);
    }
    void setQ5(int lx, int rx,int ty,int by){
        this->Q5.setLeftX(lx);
        this->Q5.setRightX(rx);
        this->Q5.setTopY(ty);
        this->Q5.setBottomY(by);
    }
    void setOGB(int lx, int rx,int ty,int by){
        this->OUTER_GRID_BOUNDARY.setLeftX(lx);
        this->OUTER_GRID_BOUNDARY.setRightX(rx);
        this->OUTER_GRID_BOUNDARY.setTopY(ty);
        this->OUTER_GRID_BOUNDARY.setBottomY(by);
    }
    void setAP1(int x,int y){this->AIMPOINT_QUAD_1.setX(x);this->AIMPOINT_QUAD_1.setY(y);}
    void setAP2(int x,int y){this->AIMPOINT_QUAD_2.setX(x);this->AIMPOINT_QUAD_2.setY(y);}
    void setAP3(int x,int y){this->AIMPOINT_QUAD_3.setX(x);this->AIMPOINT_QUAD_3.setY(y);}
    void setAP4(int x,int y){this->AIMPOINT_QUAD_4.setX(x);this->AIMPOINT_QUAD_4.setY(y);}
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
    KiteColorTracker* getKiteColorTrackingHandle(){

        return this->kiteColorTracker;
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
    float pidOutput; // corresponds to a turn value [-30,30]

    QTimer *pidTimer;

};

#endif // CONTROLALGORITHM_H
