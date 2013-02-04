#include "kitecolortracker.h"
#include <QtCore>


KiteColorTracker::KiteColorTracker(QObject *parent) :
    QObject(parent)
{
    sampleRate = 25;
    state = "idle";
    src = 0;
    winName = "camStream";
    winName2 = "Filtered Image";

    //init HSV vals
    _Hmin=11;_Smin=0;_Vmin=0;
    _Hmax=255,_Smax=255,_Vmax=255;
    _minArea=0;_maxArea=100;
    _erodeSize = 5;_dilateSize=5;

    // create timer object
    timer = new QTimer(this);
    // connect timer to update slot
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    // start timer
    timer->start(sampleRate);

    //start video capture
    if(state =="capture"){
        capture = new cv::VideoCapture();
        capture->open(src);
    //create window to display capture
    cv::namedWindow(winName,1);
    cv::namedWindow(winName2,1);
    }
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

    if(state=="capture"){

        if(capture->isOpened()){
            capture->read(currentFrame);
            cv::imshow(winName,currentFrame);

            if(!currentFrame.empty())
                filterKite(currentFrame);


        }else qDebug()<<"error acquiring webcam stream";

    }
}
void KiteColorTracker::cleanup(){

    delete capture;
    cv::destroyWindow(winName);
    cv::destroyWindow(winName2);

}

//Change HSV values for filtering. Pass in values in H-S-V order
void KiteColorTracker::setHSV(int Hmin=0, int Smin=0, int Vmin=0, int Hmax=255, int Smax=255 , int Vmax=255){
    _Hmin = Hmin;
    _Hmax = Hmax;
    _Smin = Smin;
    _Smax = Smax;
    _Vmin = Vmin;
    _Vmax = Vmax;

}

void KiteColorTracker::filterKite(cv::Mat frame){

    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    //threshold image to filter wanted colour
    cv::Mat temp,temp1;
    //filter image and save it to temp1
    cv::inRange(frame,cv::Scalar(_Hmin,_Smin,_Vmin),cv::Scalar(_Hmax,_Smax,_Vmax),temp);

    //closing of contours. we dilate and erode with little rectangles
    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(_erodeSize,_erodeSize) );
    cv::Mat element2 = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(_dilateSize,_dilateSize) );
    //dilating and erode filters out noise
    cv::erode (temp,  temp1, element);
    cv::dilate(temp1, temp1, element2 );
    cv::dilate(temp1, temp1, element2 );


    cv::imshow(winName2,temp1);
    //find contours of filtered image
    cv::findContours(temp1,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );

    //use moments method to find kite
    double px=10,py=10,pr=10;
    double refArea=0;
    if (hierarchy.size() > 0) {
        int index = 0;
        for ( ; index >= 0; index = hierarchy[index][0]) {

            cv::Moments moment = cv::moments((cv::Mat)contours[index]);
            double area = moment.m00;

            if(area >_minArea&&area<_maxArea){
                if(area>refArea){
                    refArea=area;
                    double x = moment.m10/area+1;
                    px=x;
                    double y = moment.m01/area+1;
                    py=y;
                    double r = sqrt(area/3.14);
                    pr=r;

                }
            }

        }
    }



}
void KiteColorTracker::beginCapture(){

    capture = new cv::VideoCapture();
    capture->open(src);
//create window to display capture
    cv::namedWindow(winName,1);
    cv::namedWindow(winName2,1);

    state = "capture";

}
void KiteColorTracker::endCapture(){


    capture->release();
    delete capture;
    cv::destroyWindow(winName);
    cv::destroyWindow(winName2);


    state ="idle";
}

void KiteColorTracker::setHmin(int hmin){
    _Hmin = hmin;
}
void KiteColorTracker::setHmax(int hmax){
    _Hmax = hmax;
}
void KiteColorTracker::setSmin(int smin){
    _Smin = smin;
}
void KiteColorTracker::setSmax(int smax){
    _Smax = smax;
}
void KiteColorTracker::setVmin(int vmin){
    _Vmin = vmin;
}
void KiteColorTracker::setVmax(int vmax){

    _Vmax = vmax;
}
void KiteColorTracker::setErodeSize(int size){

    _erodeSize = size;

}
void KiteColorTracker::setDilateSize(int size){

    _dilateSize = size;

}

int KiteColorTracker::getHmin(){
    return _Hmin;
}
int KiteColorTracker::getHmax(){
    return _Hmax;
}
int KiteColorTracker::getSmin(){
    return _Smin;
}
int KiteColorTracker::getSmax(){

    return _Smax;
}
int KiteColorTracker::getVmin(){

    return _Vmin;
}
int KiteColorTracker::getVmax(){

    return _Vmax;
}
int KiteColorTracker::getDilateSize(){
    return _dilateSize;
}
int KiteColorTracker::getErodeSize(){
    return _erodeSize;
}
