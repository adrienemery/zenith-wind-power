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
    // generate new path based on entry point

    // determine error

    // run pid on error value OR send error to arduino to compute pid

    // IF compute pid here THEN output turn signal to arduino
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
