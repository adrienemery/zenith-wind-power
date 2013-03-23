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
    POWER_ZONE_X=100;
    POWER_ZONE_Y=100;
    initGrid();

    // initialize PID controller
    Kp = 1.0;
    Ki =  0.0;
    Kd = 0.0;
    interval = 0.01; // in seconds

    pid = new PID(Kp,Ki,Kd,interval);
    pid->setMode(1); // 1 Automatic, 0 Manual
    pid->setSetPoint(0);    // setpoint will always be 0 degrees relative to current heading
    pid->setProcessValue(0); // initialize input to be 0 so no error
    pid->setOutputLimits(-30,30); // turning values

    // timer for pid loop
    // timer will be started by a user input that we are now tracking the kite
    pidTimer = new QTimer;
    pidTimer->setInterval(interval*1000);   // convert from seconds to msec
    connect(pidTimer,SIGNAL(timeout()),this,SLOT(updatePID()));



}
void ControlAlgorithm::initGrid(){
    FRAME_HEIGHT=kiteColorTracker->FRAME_HEIGHT;
    FRAME_WIDTH=kiteColorTracker->FRAME_WIDTH;
    OUTER_GRID_BOUNDARY.setVals(OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_Y,OUTER_GRID_OFFSET_Y);
    Q1.setVals(FRAME_WIDTH/2,FRAME_WIDTH-OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_Y,FRAME_HEIGHT/2);
    Q2.setVals(OUTER_GRID_OFFSET_X,FRAME_WIDTH/2,OUTER_GRID_OFFSET_Y,FRAME_HEIGHT/2);
    Q3.setVals(OUTER_GRID_OFFSET_X,FRAME_WIDTH/2,FRAME_HEIGHT/2,FRAME_HEIGHT-OUTER_GRID_OFFSET_Y);
    Q4.setVals(FRAME_WIDTH/2,FRAME_WIDTH-OUTER_GRID_OFFSET_X,FRAME_HEIGHT/2,FRAME_HEIGHT-OUTER_GRID_OFFSET_Y);
    //power zone
    Q5.setVals(FRAME_WIDTH/2-POWER_ZONE_X,FRAME_WIDTH/2+POWER_ZONE_X,FRAME_HEIGHT/2-POWER_ZONE_Y,FRAME_HEIGHT/2+POWER_ZONE_Y);
    //set aimpoints to middle of quadrants (for now)
    AIMPOINT_QUAD_1.setX(Q1.getLeftX()+(Q1.getRightX()-Q1.getLeftX())/2);AIMPOINT_QUAD_1.setY(Q1.getTopY()+(Q1.getBottomY()-Q1.getTopY())/2);
    AIMPOINT_QUAD_2.setX(Q2.getLeftX()+(Q2.getRightX()-Q2.getLeftX())/2);AIMPOINT_QUAD_2.setY(Q2.getTopY()+(Q2.getBottomY()-Q2.getTopY())/2);
    AIMPOINT_QUAD_3.setX(Q3.getLeftX()+(Q3.getRightX()-Q3.getLeftX())/2);AIMPOINT_QUAD_3.setY(Q3.getTopY()+(Q3.getBottomY()-Q3.getTopY())/2);
    AIMPOINT_QUAD_4.setX(Q4.getLeftX()+(Q4.getRightX()-Q4.getLeftX())/2);AIMPOINT_QUAD_4.setY(Q4.getTopY()+(Q4.getBottomY()-Q4.getTopY())/2);

}

float ControlAlgorithm::calcAngleFromVectors(QVector2D vec1,QVector2D vec2){

    float angle;
    //angle btwn 2 vectors:
    //A · B = A B cos θ = |A||B| cos θ
    //θ = cos-1(A · B/|A||B|)

    angle = acos(QVector2D::dotProduct(vec1,vec2)/
                 (sqrt(vec1.x()*vec1.x()+vec1.y()*vec1.y())*sqrt(vec2.x()*vec2.x()+vec2.y()*vec2.y())))*180/3.1416;


    return angle;

}

void ControlAlgorithm::update()
{
    //if frame height and width have changed (change capture mode perhaps?) reinitialize grid params
    int framewidth=kiteColorTracker->FRAME_WIDTH;

    if(framewidth!=FRAME_WIDTH){initGrid();}

    //get kite position
    QVector2D kitePosition = kiteColorTracker->kite->getKitePos();
    //get kite heading
    QVector2D head = kiteColorTracker->kite->getHeading();
    float headingX = head.x();
    float headingY = head.y();
    float absHead = sqrt(headingX*headingX+headingY*headingY);
    QVector2D kiteHeading(headingX/absHead,headingY/absHead);//heading unit vector




    QVector2D kitePosMem = kiteColorTracker->kite->getPosMem();
    QVector2D kiteAimPoint;
    QVector2D kiteAbsAimPoint;
    float angleError;

    // generate new path based on entry point
    if(kitePosition.x()>Q1.getLeftX()&&kitePosition.y()<Q1.getBottomY()){
        //kite is in first quadrant
        //ONLY LEFT TURN PERMITTED

        // determine error
        //get unit vector of aimpoint for error calculation
        kiteAimPoint=AIMPOINT_QUAD_3-kitePosition;
        kiteAbsAimPoint=kiteAimPoint.normalized();
        //calc error angle using vector method
        angleError=calcAngleFromVectors(kiteHeading,kiteAbsAimPoint);
        std::string turn="turn RIGHT";

        if(kiteHeading.x()<0){
            //angleError = 360-angleError;
            turn="turn LEFT";
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
            cv::line(*framePtr,cv::Point(kitePosition.x(),kitePosition.y()),cv::Point(AIMPOINT_QUAD_3.x(),AIMPOINT_QUAD_3.y()),cv::Scalar(0,255,0),2,2);

            cv::putText(*framePtr,turn+floatToStdString(angleError)+" Degrees",cv::Point(kitePosition.x(),kitePosition.y()),2,1,cv::Scalar(0,255,255),2);
        }
        // run pid on error value OR send error to arduino to compute pid

        // IF compute pid here THEN output turn signal to arduino

    }
    else if(kitePosition.x()<Q2.getRightX()&&kitePosition.y()<Q2.getBottomY()){
        //kite is in second quadrant
        //ONLY RIGHT TURN PERMITTED

        // determine error
        //get unit vector of aimpoint for error calculation
        kiteAimPoint=AIMPOINT_QUAD_4-kitePosition;
        kiteAbsAimPoint=kiteAimPoint.normalized();
        //calc error angle using vector method
        angleError=calcAngleFromVectors(kiteHeading,kiteAbsAimPoint);
        std::string turn="turn RIGHT";

        if(kiteHeading.x()<0){
            //angleError = 360-angleError;
            turn="turn LEFT";
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
            cv::line(*framePtr,cv::Point(kitePosition.x(),kitePosition.y()),cv::Point(AIMPOINT_QUAD_4.x(),AIMPOINT_QUAD_4.y()),cv::Scalar(0,255,0),2,2);

            cv::putText(*framePtr,turn+floatToStdString(angleError)+" Degrees",cv::Point(kitePosition.x(),kitePosition.y()),2,1,cv::Scalar(0,255,255),2);
        }
    }
    else if(kitePosition.x()<Q3.getRightX()&&kitePosition.y()>Q3.getTopY()){
        //kite is in third quadrant
        //ONLY RIGHT TURN PERMITTED

        // determine error
        //get unit vector of aimpoint for error calculation
        kiteAimPoint=AIMPOINT_QUAD_2-kitePosition;
        kiteAbsAimPoint=kiteAimPoint.normalized();
        //calc error angle using vector method
        angleError=calcAngleFromVectors(kiteHeading,kiteAbsAimPoint);


        std::string turn="turn LEFT";

        if(kiteHeading.x()<0){
            //angleError = 360-angleError;
            turn="turn RIGHT";
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
            cv::line(*framePtr,cv::Point(kitePosition.x(),kitePosition.y()),cv::Point(AIMPOINT_QUAD_2.x(),AIMPOINT_QUAD_2.y()),cv::Scalar(0,255,0),2,2);

            cv::putText(*framePtr,turn+floatToStdString(angleError)+" Degrees",cv::Point(kitePosition.x(),kitePosition.y()),2,1,cv::Scalar(0,255,255),2);
        }
    }
    else if(kitePosition.x()>Q4.getLeftX()&&kitePosition.y()>Q4.getTopY()){
        // determine error
        //ONLY LEFT TURN PERMITTED
        //get unit vector of aimpoint for error calculation
        kiteAimPoint=AIMPOINT_QUAD_1-kitePosition;
        kiteAbsAimPoint=kiteAimPoint.normalized();
        //calc error angle using vector method
        angleError=calcAngleFromVectors(kiteHeading,kiteAbsAimPoint);
        //decide to turn how much depending on which way kite is facing

        std::string turn="turn LEFT";

        if(kiteHeading.x()<0){
            //angleError = 360-angleError;
            turn="turn RIGHT";
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
            cv::line(*framePtr,cv::Point(kitePosition.x(),kitePosition.y()),cv::Point(AIMPOINT_QUAD_1.x(),AIMPOINT_QUAD_1.y()),cv::Scalar(0,255,0),2,2);
            cv::putText(*framePtr,turn+floatToStdString(angleError
                                                        )+" Degrees",cv::Point(kitePosition.x(),kitePosition.y()),2,1,cv::Scalar(0,255,255),2);
        }
    }

    // update PID process variable (ie. input value)
    pid->setProcessValue(angleError);  // angle error must be both posative and negative to know what side we are on


    drawToFrame(kitePosMem,kiteHeading);

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

        //draw quadrants for visualization

        cv::rectangle(*currentFrame,cv::Rect(Q1.getLeftX(),Q1.getTopY(),Q1.getRightX()-Q1.getLeftX(),Q1.getBottomY()-Q1.getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q2.getLeftX(),Q2.getTopY(),Q2.getRightX()-Q2.getLeftX(),Q2.getBottomY()-Q2.getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q3.getLeftX(),Q3.getTopY(),Q3.getRightX()-Q3.getLeftX(),Q3.getBottomY()-Q3.getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q4.getLeftX(),Q4.getTopY(),Q4.getRightX()-Q4.getLeftX(),Q4.getBottomY()-Q4.getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q5.getLeftX(),Q5.getTopY(),Q5.getRightX()-Q5.getLeftX(),Q5.getBottomY()-Q5.getTopY()),cv::Scalar(100,100,255));

        // currentFrame=NULL;
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

    // TODO: overload constructors and copy constructors ??
    delete this->imageProcessingWindow;
    delete this->pid;
    pid = NULL;
    imageProcessingWindow = NULL;

}

void ControlAlgorithm::startPidTimer()
{
    pidTimer->start();
}

// updates PID every interval of the pidTimer QTimer
void ControlAlgorithm::updatePID()
{
   pidOutput = pid->compute();

}
