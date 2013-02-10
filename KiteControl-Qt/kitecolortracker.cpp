#include "kitecolortracker.h"

#include <QString>
#include <QtCore>
#include <QDir>
#include <QFile>
#include <QTextStream>





KiteColorTracker::KiteColorTracker(QObject *parent) :
    QObject(parent)
{
    sampleRate = 25;
    state = "idle";
    src = 0;
    winName = "camStream";
    winName2 = "Filtered Image";

    //load all values
    bool success = loadFilterData("filterData.txt");
    _showDilateErode = false;
    _showRFI = false; //show raw filtered image

    //error thresholds for kite tracking with servo/webcam setup
    //TODO: have these accessible from UI
    _minErrorX=40;
    _minErrorY=40;
//    _panVal = 90;
//    _tiltVal = 90;
    _x = 0;
    _y = 0;

    _propGain = 1;

    _trackKite = false;

    // create timer object
    timer = new QTimer(this);
    // connect timer to update slot
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    // start timer
    timer->start(sampleRate);
    \
    //create new capture object
    capture = new cv::VideoCapture();

    _dataLoggerFileCreated = false;

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
    //copy newest webcam image

    if(state=="capture"){



        if(capture->isOpened()){

            bool checkFrame = false;
            if(_play)//if paused, don't capture next frame
                checkFrame = capture->read(currentFrame);

            if(checkFrame){

                filterKite(currentFrame);

                cv::imshow(winName,currentFrame);}

        }else{ //qDebug()<<"error acquiring video stream!";

            this->endCapture();
        }

    }
    //send signal to say new position data is ready
    emit dataUpdated();
}
void KiteColorTracker::cleanup(){

    if(capture->isOpened())
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
    std::string rfiWin = "RAW FILTERED IMAGE";
    if(_showRFI)cv::imshow(rfiWin,temp);
    else cv::destroyWindow(rfiWin);
    cv::erode (temp,  temp1, element);
    std::string erodeWin = "AFTER ERODING";
    if(_showDilateErode)cv::imshow(erodeWin,temp1);
    else cv::destroyWindow(erodeWin);
    cv::dilate(temp1, temp1, element2 );
    cv::dilate(temp1, temp1, element2 );



    cv::imshow(winName2,temp1);
    //find contours of filtered image
    cv::findContours(temp1,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );

    //use moments method to find kite
    //double px=10,py=10,pr=10;
    double px=CAM_CENTER_X, py=CAM_CENTER_Y, pr=10;
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
                    _x = x; // update x position for data entry
                    double y = moment.m01/area+1;
                    py=y;
                    _y = y; // update y postion for data entry
                    double r = sqrt(area/3.14);
                    pr=r;
                    qDebug() << "Kite Found.";
                }
            }
            else{
                px=CAM_CENTER_X;
                py=CAM_CENTER_Y;
            }
        }
    }

    //now we adjust webcam according to these values
    //with respect to the center of the frame

    //only track kite if told to
    if(_trackKite){
        adjustCamPosition(px,py);
        dataLogger();
    }

}
void KiteColorTracker::adjustCamPosition(int x, int y){

    //calculate error between (xcenter,ycenter) and (xcurrent,ycurrent)
    // calculate errorx and errory
    int errorx,errory;
    errorx = x - CAM_CENTER_X;
    errory = y - CAM_CENTER_Y;

    qDebug() << "errors " <<  errorx << errory;

    // check if error is bigger than minimum
    if( abs(errorx) > _minErrorX )
    {
        int stepSize;

        stepSize = int( (abs(float(errorx)) / CAM_CENTER_X) * 5 );
        //stepSize = float(60/640)*errorx;
        //qDebug()<<"stepsize: "<<QString::number(stepSize);
        if(stepSize == 0) stepSize = 1;
        // tell arduino to pan camera to minimize error
        if(errorx > 0)
        {
            // pan right with respect to last pan value
            _panVal = _panVal - stepSize;
            //qDebug()<<"PAN VALUE RIGHT: "<<_panVal;
            //restrict max and min pan values to 180 and 0
            if(_panVal > 180) _panVal = 180;
            if(_panVal < 0) _panVal = 0;

            emit writeToArduino("P" + QString::number(_panVal)+"/");


        }
        else
        {
            // pan left
            _panVal = _panVal + stepSize;
            //qDebug()<<"PAN VALUE LEFT: "<<_panVal;
            if(_panVal > 180) _panVal = 180;
            if(_panVal < 0) _panVal = 0;
            emit writeToArduino("P" + QString::number(_panVal) + "/");
        }
    }
    if( abs(errory) > _minErrorY)
    {
        int stepSize;
        stepSize = int( (abs(float(errory)) / CAM_CENTER_Y) * 5  );
        //stepSize = float(46.83/480)*errory;

        if(stepSize == 0) stepSize = 1;

        // tell arduino to tilt camera to minimize error
        if(errory > 0)
        {
            // tilt down
            _tiltVal = _tiltVal + stepSize;
            //qDebug()<<"TILT VALUE DOWN: "<<_tiltVal;
            if(_tiltVal > 180) _tiltVal = 180;
            if(_tiltVal < 0) _tiltVal = 0;
            emit writeToArduino("T" + QString::number(_tiltVal) + "/");
        }
        else
        {
            // tilt up
            _tiltVal = _tiltVal - stepSize;
            //qDebug()<<"TILT VALUE UP: "<<_tiltVal;
            if(_tiltVal > 180) _tiltVal = 180;
            if(_tiltVal < 0) _tiltVal = 0;
            emit writeToArduino("T" + QString::number(_tiltVal) + "/");

        }
    }

}


void KiteColorTracker::beginCapture(std::string capType){


    //find current directory
    QString cPath= QDir::currentPath();
    //create QDir object with current directory
    QDir theDir( cPath);
    //move UP a folder
    bool pathExists = theDir.cdUp();
    if(pathExists)
        theDir.cd("KiteControl-Qt/videos");
    qDebug()<<"Directory Changed to: "+theDir.path();
    std::string vidPath = theDir.path().toStdString();

    if(capType=="camera"){

        capture->open(src);
        capture->set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
        capture->set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);}
    else if (capType=="movie")

        capture->open(vidPath+"/kiteTest.avi");

    //create window to display capture
    cv::namedWindow(winName,1);
    cv::namedWindow(winName2,1);
    //switch state to capture
    //init cam position
    emit writeToArduino("P"+QString::number(_panVal)+"/");
    emit writeToArduino("T"+QString::number(_tiltVal)+"/");
    state = "capture";


}
void KiteColorTracker::endCapture(){



    capture->release();
    cv::destroyWindow(winName);
    cv::destroyWindow(winName2);
    //switch to idle state (no webcam stream)
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
void KiteColorTracker::setEDflag(bool flag){


    //this just says whether or not to show the frames
    //for dilating and eroding
    _showDilateErode = flag;

}
void KiteColorTracker::setRFIflag(){

    _showRFI = !_showRFI;
}

void KiteColorTracker::setMaxArea(int max){

    _maxArea = max;
}
void KiteColorTracker::setMinArea(int min){
    _minArea = min;
}
void KiteColorTracker::playPause(){

    _play = !_play;
}
void KiteColorTracker::toggleTrackKite(){


    _trackKite =!_trackKite;
}
void KiteColorTracker::setPropGain(int gain){

    _propGain = gain;
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
int KiteColorTracker::getMinArea(){
    return _minArea;
}
int KiteColorTracker::getMaxArea(){
    return _maxArea;
}

void KiteColorTracker::save(QString fileName){
    //save all filtering settings to file
    //find current directory
    QString cPath= QDir::currentPath();
    //create QDir object with current directory
    QDir theDir( cPath);
    //move UP a folder
    bool pathExists = theDir.cdUp();
    if(pathExists)
        theDir.cd("KiteControl-Qt/filterData");
    else qDebug()<<"error changing path";

    QString savePath = theDir.path()+"/"+fileName;
    qDebug()<<"Saving Data to: "+savePath;

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    //save HSV values, Area, Erode, Dilate
    QTextStream out(&file);
    out << "H " << intToString(getHmin())+" "+intToString(getHmax())<<"\n";
    out << "S " << intToString(getSmin())+" "+intToString(getSmax())<<"\n";
    out << "V " << intToString(getVmin())+" "+intToString(getVmax())<<"\n";
    out << "A " << intToString(getMinArea()) + " "+intToString(getMaxArea())<<"\n";
    out << "E " << intToString(getErodeSize())<<"\n";
    out << "D " << intToString(getDilateSize())<<"\n";




}
bool KiteColorTracker::loadFilterData(QString fileName){
    //returns true if file loaded successfully
    //find current directory
    QString cPath= QDir::currentPath();
    //create QDir object with current directory
    QDir theDir( cPath);
    //move UP a folder
    bool pathExists = theDir.cdUp();
    if(pathExists)
        theDir.cd("KiteControl-Qt/filterData");
    else qDebug()<<"error changing path";

    QString savePath = theDir.path()+"/"+fileName;
    qDebug()<<"Loading data from: "+savePath+".....";

    QFile file(savePath);
    qDebug()<<"created file";
    bool openFile = file.open(QIODevice::ReadOnly | QIODevice::Text);
    qDebug()<<openFile;
    if (openFile==0)
    { qDebug()<<"error loading"+savePath;
        return false;}


    QTextStream in(&file);
    QChar ch;

    qDebug()<<in.atEnd();
    while (!in.atEnd()) {

        //create temp char
        in>>ch;
        //if H is found, then save the next 2 H vals
        if(ch=='H'){
            int hmin,hmax;
            in>>hmin>>hmax;

            setHmin(hmin);setHmax(hmax);

        }
        else if (ch=='S'){

            int smin,smax;
            in>>smin>>smax;
            setSmin(smin); setSmax(smax);
        }
        else if (ch=='V'){

            int vmin,vmax;
            in>>vmin>>vmax;
            setVmin(vmin);setVmax(vmax);
        }
        else if (ch=='A'){

            int amin,amax;
            in>>amin>>amax;
            setMinArea(amin);setMaxArea(amax);
        }
        else if (ch=='E'){

            int esize;
            in>>esize;
            setErodeSize(esize);
        }
        else if (ch=='D'){

            int dsize;
            in>>dsize;
            setDilateSize(dsize);
        }

    }

    //      _Hmin=11;_Smin=0;_Vmin=0;
    //      _Hmax=255,_Smax=255,_Vmax=255;
    //      _minArea=0;_maxArea=10000;
    //      _erodeSize = 5;_dilateSize=5;

    return pathExists;
}

bool KiteColorTracker::isPaused()
{
    if(_play){
        return false;
    }else{
        return true;
    }
    return true;
}

void KiteColorTracker::setMinErrorX(int val)
{
    //_minErrorX = val;
}

void KiteColorTracker::setMinErrorY(int val)
{
    //_minErrorY = val;
}

void KiteColorTracker::dataLogger()
{
    //save all filtering settings to file
    //find current directory
    QString cPath= QDir::currentPath();
    //create QDir object with current directory
    QDir theDir(cPath);
    //move UP a folder
    bool pathExists = theDir.cdUp();
    if(pathExists)
        theDir.cd("KiteControl-Qt/data");
    else qDebug()<<"error changing path";

    QString savePath = theDir.path() + "/" + "trackingData.txt";
    //qDebug()<<"Saving Data to: " + savePath;

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        // do something
    }

    QTextStream out(&file);
    // write column titles the first time through
    if(!_dataLoggerFileCreated) {
        out << "x" << "," << "y" << "," << "pan" << "," << "tilt" << "," << "time" << "\n";
        _dataLoggerFileCreated = true;
    }

    // write data to the file
    out << QString::number(_x)  << "," << QString::number(_y) << "," << QString::number(_panVal) << "," << QString::number(_tiltVal) << "," << QString::number(sampleRate)  << "\n";

}

void KiteColorTracker::setPanVal(int val)
{
    _panVal = val;
    emit writeToArduino("P" + QString::number(_panVal) + "/");

}

void KiteColorTracker::setTiltVal(int val)
{
    _tiltVal = val;
    emit writeToArduino("T" + QString::number(_tiltVal) + "/");

}
