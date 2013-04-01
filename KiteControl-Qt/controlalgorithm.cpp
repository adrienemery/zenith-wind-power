#include "controlalgorithm.h"

ControlAlgorithm::ControlAlgorithm(QObject *parent) :
    QObject(parent)
{

    //TODO: point to already created kiteColorTracker instead of making new one
    // kiteColorTracker = new KiteColorTracker(this);

    imageProcessingWindow = new ImageProcessing();


    kiteColorTracker = imageProcessingWindow->getColorTracker();
    kiteColorTracker->setSampleRate(1);

    // call update whenever a new position computed by kiteColorTracker
    connect(kiteColorTracker,SIGNAL(dataUpdated()),this,SLOT(update()));


    // initialize start and end points for left/right paths



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
    pid->setMode(1.0); // 1 Automatic, 0 Manual
    pid->setSetPoint(0.0);    // setpoint will always be 0 degrees relative to current heading
    pid->setInputLimits(0.0,45.0);
    pid->setOutputLimits(0.0,30.0); // turning values
    pid->setProcessValue(10.0); // initialize input to be 0 so no error


    // timer for pid loop
    // timer will be started by a user input that we are now tracking the kite
    pidTimer = new QTimer;
    pidTimer->setInterval(interval*1000);   // convert from s to ms
    connect(pidTimer,SIGNAL(timeout()),this,SLOT(updatePID()));
    //pidTimer->start();

    autoPilotOn = false;

}
void ControlAlgorithm::initGrid(){

    Q1 = new Quadrant;Q2 = new Quadrant;Q3 = new Quadrant;Q4 = new Quadrant;Q5 = new Quadrant;OUTER_GRID_BOUNDARY = new Quadrant;
    qDebug()<<"GRID UPDATED?";
    FRAME_HEIGHT=kiteColorTracker->FRAME_HEIGHT;
    FRAME_WIDTH=kiteColorTracker->FRAME_WIDTH;
    OUTER_GRID_BOUNDARY->setVals(OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_Y,OUTER_GRID_OFFSET_Y);
    Q1->setVals(FRAME_WIDTH/2,FRAME_WIDTH-OUTER_GRID_OFFSET_X,OUTER_GRID_OFFSET_Y,FRAME_HEIGHT/2);
    Q2->setVals(OUTER_GRID_OFFSET_X,FRAME_WIDTH/2,OUTER_GRID_OFFSET_Y,FRAME_HEIGHT/2);
    Q3->setVals(OUTER_GRID_OFFSET_X,FRAME_WIDTH/2,FRAME_HEIGHT/2,FRAME_HEIGHT-OUTER_GRID_OFFSET_Y);
    Q4->setVals(FRAME_WIDTH/2,FRAME_WIDTH-OUTER_GRID_OFFSET_X,FRAME_HEIGHT/2,FRAME_HEIGHT-OUTER_GRID_OFFSET_Y);
    //power zone
    Q5->setVals(FRAME_WIDTH/2-POWER_ZONE_X,FRAME_WIDTH/2+POWER_ZONE_X,FRAME_HEIGHT/2-POWER_ZONE_Y,FRAME_HEIGHT/2+POWER_ZONE_Y);
    //set aimpoints to middle of quadrants (for now)
    AIMPOINT_QUAD_1 = new QVector2D;AIMPOINT_QUAD_2 = new QVector2D;AIMPOINT_QUAD_3 = new QVector2D;AIMPOINT_QUAD_4 = new QVector2D;
    AIMPOINT_QUAD_1->setX(Q1->getLeftX()+(Q1->getRightX()-Q1->getLeftX())/2);AIMPOINT_QUAD_1->setY(Q1->getTopY()+(Q1->getBottomY()-Q1->getTopY())/2);
    AIMPOINT_QUAD_2->setX(Q2->getLeftX()+(Q2->getRightX()-Q2->getLeftX())/2);AIMPOINT_QUAD_2->setY(Q2->getTopY()+(Q2->getBottomY()-Q2->getTopY())/2);
    AIMPOINT_QUAD_3->setX(Q3->getLeftX()+(Q3->getRightX()-Q3->getLeftX())/2);AIMPOINT_QUAD_3->setY(Q3->getTopY()+(Q3->getBottomY()-Q3->getTopY())/2);
    AIMPOINT_QUAD_4->setX(Q4->getLeftX()+(Q4->getRightX()-Q4->getLeftX())/2);AIMPOINT_QUAD_4->setY(Q4->getTopY()+(Q4->getBottomY()-Q4->getTopY())/2);

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
    //if frame height and width have changed (changed capture mode perhaps?) reinitialize grid params
    int framewidth=kiteColorTracker->FRAME_WIDTH;

    if(framewidth!=FRAME_WIDTH){initGrid();}

    //get kite position
    QVector2D tempVec = kiteColorTracker->kite->getKitePos();
    kitePosition = &tempVec;
    //get kite heading
    QVector2D head = kiteColorTracker->kite->getHeading();
    float headingX = head.x();
    float headingY = head.y();
    float absHead = sqrt(headingX*headingX+headingY*headingY);
    QVector2D kiteHeading(headingX/absHead,headingY/absHead);  //heading unit vector

    qlineKiteHeading.setP1(QPointF(kitePosition->x(),kitePosition->y()));
    qlineKiteHeading.setP2(QPointF(kitePosition->x()+kiteHeading.x(),kitePosition->y()+kiteHeading.y()));


    //we know first point of kiteAimpoint will be the kite itself
    qlineAimPoint.setP1(QPointF(kitePosition->x(),kitePosition->y()));

    QVector2D kitePosMem = kiteColorTracker->kite->getPosMem();
    QVector2D kiteAimPoint;

    float angleError;
    bool turnRight;
    // generate new path based on entry point
    if(kitePosition->x()>Q1->getLeftX()&&kitePosition->y()<Q1->getBottomY()){
        //kite is in first quadrant
        //ONLY LEFT TURN PERMITTED

        kiteAimPoint=*AIMPOINT_QUAD_3;
        //set P2 qline from kite to aimpoint
        qlineAimPoint.setP2(QPointF(kiteAimPoint.x(),kiteAimPoint.y()));
        //calc error angle using qline angles for full 360 deg. range
        angleError= qlineKiteHeading.angleTo(qlineAimPoint);

        if(angleError>180){
            angleError-=360;
            turnRight=false;
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
            cv::line(*framePtr,cv::Point(qlineAimPoint.x1(),qlineAimPoint.y1()),cv::Point(qlineAimPoint.x2(),qlineAimPoint.y2()),cv::Scalar(0,255,0),2,2);

            cv::putText(*framePtr,floatToStdString(angleError)+" Degrees",cv::Point(kitePosition->x(),kitePosition->y()),2,1,cv::Scalar(0,255,255),2);
        }

    }
    else if(kitePosition->x()<Q2->getRightX()&&kitePosition->y()<Q2->getBottomY()){
        //kite is in second quadrant
        //ONLY RIGHT TURN PERMITTED

        kiteAimPoint=*AIMPOINT_QUAD_4;
        //set P2 qline from kite to aimpoint
        qlineAimPoint.setP2(QPointF(kiteAimPoint.x(),kiteAimPoint.y()));
        //calc error angle using qline angles for full 360 deg. range
        angleError= qlineKiteHeading.angleTo(qlineAimPoint);

        if(angleError>180){
            angleError-=360;
            turnRight=false;
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
            cv::line(*framePtr,cv::Point(qlineAimPoint.x1(),qlineAimPoint.y1()),cv::Point(qlineAimPoint.x2(),qlineAimPoint.y2()),cv::Scalar(0,255,0),2,2);
            cv::putText(*framePtr,floatToStdString(angleError)+" Degrees",cv::Point(kitePosition->x(),kitePosition->y()),2,1,cv::Scalar(0,255,255),2);
        }
        // run pid on error value OR send error to arduino to compute pid

        // IF compute pid here THEN output turn signal to arduino
    }
    else if(kitePosition->x()<Q3->getRightX()&&kitePosition->y()>Q3->getTopY()){
        //kite is in third quadrant
        //ONLY RIGHT TURN PERMITTED

        kiteAimPoint=*AIMPOINT_QUAD_2;
        //set P2 qline from kite to aimpoint
        qlineAimPoint.setP2(QPointF(kiteAimPoint.x(),kiteAimPoint.y()));
        //calc error angle using qline angles for full 360 deg. range
        angleError= qlineKiteHeading.angleTo(qlineAimPoint);

        if(angleError>180){
            angleError-=360;
            turnRight=false;
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
               cv::line(*framePtr,cv::Point(qlineAimPoint.x1(),qlineAimPoint.y1()),cv::Point(qlineAimPoint.x2(),qlineAimPoint.y2()),cv::Scalar(0,255,0),2,2);
            cv::putText(*framePtr,floatToStdString(angleError)+" Degrees",cv::Point(kitePosition->x(),kitePosition->y()),2,1,cv::Scalar(0,255,255),2);
        }
        // run pid on error value OR send error to arduino to compute pid

        // IF compute pid here THEN output turn signal to arduino
    }
    else if(kitePosition->x()>Q4->getLeftX()&&kitePosition->y()>Q4->getTopY()){
        // determine error
        //ONLY LEFT TURN PERMITTED
        kiteAimPoint=*AIMPOINT_QUAD_1;
        //set P2 qline from kite to aimpoint
        qlineAimPoint.setP2(QPointF(kiteAimPoint.x(),kiteAimPoint.y()));
        //calc error angle using qline angles for full 360 deg. range
        angleError=qlineAimPoint.angle()-qlineKiteHeading.angle();

        angleError= qlineKiteHeading.angleTo(qlineAimPoint);

        if(angleError>180){
            angleError-=360;
        }

        cv::Mat *framePtr = kiteColorTracker->getFrameHandle();
        //draw line from kite to AimPoint
        if(framePtr!=NULL){
               cv::line(*framePtr,cv::Point(qlineAimPoint.x1(),qlineAimPoint.y1()),cv::Point(qlineAimPoint.x2(),qlineAimPoint.y2()),cv::Scalar(0,255,0),2,2);
            cv::putText(*framePtr,floatToStdString(angleError)+" Degrees",cv::Point(kitePosition->x(),kitePosition->y()),2,1,cv::Scalar(0,255,255),2);
        }
        // run pid on error value OR send error to arduino to compute pid

        // IF compute pid here THEN output turn signal to arduino
    }

    // update PID process variable (ie. input value)
    pid->setProcessValue(abs(angleError));  // angle error must be both posative and negative to know what side we are on

    // compute new pid output
    updatePID();

    if(angleError<0){pidOutput=-pidOutput;}
    //qDebug()<<"PID output: "<<pidOutput;

    drawToFrame(kitePosMem,kiteHeading);

    //save position data for next interation
    kiteColorTracker->kite->setPosMem(kitePosition->x(),kitePosition->y());
    //emit data for control options window display
    emit(dataUpdated());

    //finally, emit signal for turn command to kite
    //if autopilot is engaged
    if(autoPilotOn)
    emit(writeToArduino("T "+QString::number(pidOutput)));


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

        cv::rectangle(*currentFrame,cv::Rect(Q1->getLeftX(),Q1->getTopY(),Q1->getRightX()-Q1->getLeftX(),Q1->getBottomY()-Q1->getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q2->getLeftX(),Q2->getTopY(),Q2->getRightX()-Q2->getLeftX(),Q2->getBottomY()-Q2->getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q3->getLeftX(),Q3->getTopY(),Q3->getRightX()-Q3->getLeftX(),Q3->getBottomY()-Q3->getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q4->getLeftX(),Q4->getTopY(),Q4->getRightX()-Q4->getLeftX(),Q4->getBottomY()-Q4->getTopY()),cv::Scalar(0,255,255));
        cv::rectangle(*currentFrame,cv::Rect(Q5->getLeftX(),Q5->getTopY(),Q5->getRightX()-Q5->getLeftX(),Q5->getBottomY()-Q5->getTopY()),cv::Scalar(100,100,255));

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

void ControlAlgorithm::setPidParams(float Kp, float Ki, float Kd)
{
    pid->setTunings(Kp,Ki,Kd);

}

void ControlAlgorithm::setPidInput(float input)
{
    pid->setProcessValue(input);
}

void ControlAlgorithm::setPidSetpoint(float setpoint)
{
    pid->setSetPoint(setpoint);
}

void ControlAlgorithm::setPidInterval(int msec)
{
    pid->setInterval(float(msec)/1000.0);
    pidTimer->setInterval(msec);
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

void ControlAlgorithm::stopPidTimer()
{
    pidTimer->stop();
}

// updates PID every interval of the pidTimer QTimer
void ControlAlgorithm::updatePID()
{
   pidOutput = pid->compute();


}
