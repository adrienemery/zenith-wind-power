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

    //set quadrant parameters
       OUTER_GRID_OFFSET_X=100;
       OUTER_GRID_OFFSET_Y=50;
       POWER_ZONE_X==100;
       POWER_ZONE_Y=100;
       int frameheight=kiteColorTracker->FRAME_HEIGHT;
       int framewidth=kiteColorTracker->FRAME_WIDTH;
       OUTER_GRID_BOUNDARY.setVals(OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_Y,OUTER_GRID_OFFSET_Y);
       Q1.setVals(framewidth/2,framewidth-OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_Y,frameheight/2);
       Q2.setVals(OUTER_GRID_OFFSET_X,framewidth/2,OUTER_GRID_OFFSET_Y,frameheight/2);
       Q3.setVals(OUTER_GRID_OFFSET_X,framewidth/2,frameheight/2,frameheight-OUTER_GRID_OFFSET_Y);
       Q4.setVals(framewidth/2,framewidth-OUTER_GRID_OFFSET_X,frameheight/2,frameheight-OUTER_GRID_OFFSET_X);
       Q5.setVals(framewidth/2-POWER_ZONE_X,framewidth/2+POWER_ZONE_X,frameheight/2-POWER_ZONE_Y,frameheight/2+POWER_ZONE_Y);

}

void ControlAlgorithm::update()
{

    //get kite position
    QVector2D kitePosition = kiteColorTracker->kite->getKitePos();
       //get kite heading
      QVector2D head = kiteColorTracker->kite->getHeading();
      float headingX = head.x();
      float headingY = head.y();
      float absHead = sqrt(headingX*headingX+headingY*headingY);
      QVector2D heading(headingX/absHead,headingY/absHead);//heading unit vector





       QVector2D kitePosMem = kiteColorTracker->kite->getPosMem();


       // generate new path based on entry point
       if(kitePosition.x()>Q1.getLeftX()&&kitePosition.y()<Q1.getBottomY()){
           //kite is in first quadrant
           //ONLY LEFT TURN PERMITTED

           // determine error


           // run pid on error value OR send error to arduino to compute pid

           // IF compute pid here THEN output turn signal to arduino

       }
      else if(kitePosition.x()<Q2.getRightX()&&kitePosition.y()<Q2.getBottomY()){
           //kite is in second quadrant
           //ONLY RIGHT TURN PERMITTED

           // determine error

           // run pid on error value OR send error to arduino to compute pid

           // IF compute pid here THEN output turn signal to arduino

       }
      else if(kitePosition.x()<Q3.getRightX()&&kitePosition.y()>Q3.getTopY()){
           //kite is in third quadrant
           //ONLY RIGHT TURN PERMITTED

           // determine error

           // run pid on error value OR send error to arduino to compute pid

           // IF compute pid here THEN output turn signal to arduino

       }
      else if(kitePosition.x()>Q4.getLeftX()&&kitePosition.y()>Q4.getTopY()){
           //kite is in fourth quadrant
           //ONLY LEFT TURN PERMITTED

           // determine error

           // run pid on error value OR send error to arduino to compute pid

           // IF compute pid here THEN output turn signal to arduino

       }








        drawToFrame(kitePosMem,heading);



              //save position data for next interation
      kiteColorTracker->kite->setPosMem(kitePosition.x(),kitePosition.y());


}

void ControlAlgorithm::kiteControlAlgorithm(){



}
void ControlAlgorithm::drawToFrame(QVector2D kitePos, QVector2D heading){
    kiteTracer.push_back(QVector2D(kitePos.x(),kitePos.y()));

    if(kiteTracer.size()>150){

        kiteTracer.pop_front();


    }

    //length of visual line vector
    int lineLength = 120;
    //still use recent kite x,y to keep an eye on the actual position
    //of kite regardless if it has moved out of the bounding rect or not


    //get handle to currentFrame so we can
    //manipulate stuff on the image (draw paths etc.)
    currentFrame = kiteColorTracker->getFrameHandle();

    //test
    if(currentFrame!=NULL){


        cv::line(*currentFrame,cv::Point(kitePos.x(),kitePos.y()),cv::Point(kitePos.x()+lineLength*heading.x(),kitePos.y()+lineLength*heading.y()),
                 cv::Scalar(0,0,255),4);


        cv::Point pointmem(0,0);
        if(kiteTracer.size()>0)
        pointmem=cv::Point(kiteTracer.at(0).x(),kiteTracer.at(0).y());
        for(int j=0;j<kiteTracer.size();j++){

            cv::line(*currentFrame,pointmem,cv::Point(kiteTracer.at(j).x(),kiteTracer.at(j).y()),cv::Scalar(0,255,0),1);
            cv::circle(*currentFrame,cv::Point(kiteTracer.at(j).x(),kiteTracer.at(j).y()),2,cv::Scalar(255,0,0),1);
            pointmem = cv::Point(kiteTracer.at(j).x(),kiteTracer.at(j).y());
        }


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
