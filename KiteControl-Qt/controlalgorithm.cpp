#include "controlalgorithm.h"

ControlAlgorithm::ControlAlgorithm(QObject *parent) :
    QObject(parent)
{

    //TODO: point to already created kiteColorTracker instead of making new one
   // kiteColorTracker = new KiteColorTracker(this);

    imageProcessingWindow = new ImageProcessing();

    kiteColorTracker = imageProcessingWindow->getColorTracker();

    // call update whenever a new position computed by kiteColorTracker
    connect(kiteColorTracker,SIGNAL(dataUpdated()),this,SLOT(update()));


    // initialize start and end points for left/right paths

    // calculate left/right paths
    leftPath = leftBegin - leftEnd;

    // set min/max bounds



}

void ControlAlgorithm::update()
{

   QVector2D head = kiteColorTracker->kite->getHeading();
   float headingX = head.x();
   float headingY = head.y();
   float absHead = sqrt(headingX*headingX+headingY*headingY);

   //create unit vector to represent direction
   QVector2D heading(headingX/absHead,headingY/absHead);



   QVector2D kitePos = kiteColorTracker->kite->getKitePos();


    // generate new path based on entry point

    // determine error

    // run pid on error value OR send error to arduino to compute pid

    // IF compute pid here THEN output turn signal to arduino



     drawToFrame(kitePos,heading);





}

void ControlAlgorithm::kiteControlAlgorithm(){



}
void ControlAlgorithm::drawToFrame(QVector2D kitePos, QVector2D heading){
    //get handle to currentFrame so we can
    //manipulate stuff on the image (draw paths etc.)

    int lineLength = 80;
    currentFrame = kiteColorTracker->getFrameHandle();

    //test
    if(currentFrame!=NULL){


        cv::line(*currentFrame,cv::Point(kitePos.x(),kitePos.y()),cv::Point(kitePos.x()+lineLength*heading.x(),kitePos.y()+lineLength*heading.y()),
                 cv::Scalar(0,255,0),3);




    }

}

void ControlAlgorithm::setMinX(float x)
{
    minX = x;
}

void ControlAlgorithm::setMaxX(float x)
{
    maxX = x;
}

void ControlAlgorithm::setMinY(float y)
{
    minY = y;
}

void ControlAlgorithm::setMaxY(float y)
{
    maxY = y;
}
ControlAlgorithm::~ControlAlgorithm(){

    delete this->imageProcessingWindow;
    imageProcessingWindow = NULL;

}
