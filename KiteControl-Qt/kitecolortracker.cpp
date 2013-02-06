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

    // create timer object
    timer = new QTimer(this);
    // connect timer to update slot
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    // start timer
    timer->start(sampleRate);
    \
    //create new capture object
    capture = new cv::VideoCapture();

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

        }else{ qDebug()<<"error acquiring video stream!";

            this->endCapture();
        }

    }
   //send signal to say new data is ready
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

    //draw box around kite
    cv::circle(frame,cv::Point(px,py),3,cv::Scalar(0,0,255),3);
    //show some info text beside it
   // cv::putText(frame,("("+QString::toStdString(intToString(px))+","+QString::toStdString(intToString(py))+")"),cv::Point(px+50,py),1,2,cv::Scalar(0,0,255),2);


    //qDebug()<<"area: "<<refArea<<"\n"<<"(x,y): "<<px<<" , "<<py;

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

    if(capType=="camera")
    capture->open(src);
    else if (capType=="movie")

        capture->open(vidPath+"/kiteTest.avi");

    //create window to display capture
    cv::namedWindow(winName,1);
    cv::namedWindow(winName2,1);
    //switch state to capture
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
             else if (ch=="S"){

                 int smin,smax;
                 in>>smin>>smax;
                 setSmin(smin); setSmax(smax);
             }
             else if (ch=="V"){

                 int vmin,vmax;
                 in>>vmin>>vmax;
                 setVmin(vmin);setVmax(vmax);
             }
             else if (ch=="A"){

                 int amin,amax;
                 in>>amin>>amax;
                 setMinArea(amin);setMaxArea(amax);
             }
             else if (ch=="E"){

                 int esize;
                 in>>esize;
                 setErodeSize(esize);
             }
             else if (ch=="D"){

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

