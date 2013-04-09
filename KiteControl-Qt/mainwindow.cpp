#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QScrollBar>
#include <QMessageBox>
#include <QDateTime>
#include <QTime>
#include "powerinfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // initialize main window ui
    ui->setupUi(this);

    // additional ui initializations
    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup()
{


    // initialize child Windows
    powerInfoWindow = new PowerInfo(this);
    addKiteWindow = new AddKite(this);

    controlWindow = new ControlWindow(this);
    kiteController = controlWindow->getControlAlgHandle();
    handshakeTimer = new QTimer(this);
    connect(addKiteWindow,SIGNAL(kiteAdded()),this,SLOT(addKiteToComboBox()));
    //connect(this,SIGNAL(serialReady()),imageProcessingWindow->getColorTracker(),SLOT(serialReady()));
    //connect(imageProcessingWindow,SIGNAL(writeToArduino(QString)),this,SLOT(writeToArduino(QString)));

    connect(kiteController,SIGNAL(writeToArduino(QString)),this,SLOT(writeToArduino(QString)));
    connect(ui->serialInput,SIGNAL(returnPressed()),this,SLOT(on_serialSendButton_clicked()));

    // start Qtimer to poll joystick values every 15ms
    tmr.setInterval(30);
    connect(&tmr,SIGNAL(timeout()),this,SLOT(readJoystickState()));
    tmr.start();

    handshakeTimer->setInterval(100);
    handshakeTimer->setSingleShot(true);
    connect(handshakeTimer,SIGNAL(timeout()),this,SLOT(on_forceHandshakeButton_clicked()));


    connect(ui->imgProcButton,SIGNAL(clicked()),ui->actionImage_Processor,SLOT(trigger()));

    // load saved data
    Load();

    // initialize port info

    port = new QextSerialPort("COM3");

    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_2);

    // open port and check if it opened properly
    if(port->open(QextSerialPort::ReadWrite)){
        ui->serialStatusLabel->setText("<font color='green'>CONNECTED </font>");

    }else{
        ui->serialStatusLabel->setText("<font color='red'>NOT CONNECTED</font>");
        port->close();
    }

    // add ports to port drop down menu
    QList<QextPortInfo> portInfo = QextSerialEnumerator::getPorts();
    for(int i =0; i<portInfo.size(); i++){
        ui->portMenu->addItem(portInfo.value(i).portName);
    }

    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.value(i).portName.contains("usb") || portInfo.value(i).portName.contains("3") ){
            ui->portMenu->setCurrentIndex(i);
        }
    }

    ui->baudRateMenu->setCurrentIndex(1);

    // get local ip address
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());

    // setup joystick
    f_haveJoystick = joystick.initInput(0);
    if (!f_haveJoystick)
    {
        this->setWindowTitle(QString("%1 :: %2").arg(this->windowTitle()).arg("Joystick not found"));
    }
    else
    {
        this->setWindowTitle(QString("%1 :: %2").arg(this->windowTitle()).arg("Joystick present"));
    }

    // initialize current state information
    ui->currentModeLabel->setText("Manual Mode");
    autoPilotOn = false;
    inCalmode = false;


    arduinoReady = true;
    _dataLoggerFileCreated = false;
}

// **********Serial Functions**********

bool MainWindow::writeToArduino(QString msg)
{

    if(port->isOpen() && arduinoReady){

        QByteArray bytes;
        bytes.append(msg);

        port->write(bytes);
        port->flush();
        arduinoReady = false;
        handshakeTimer->start();
        bTx = true;
        writeToSerialMonitor(msg);    // Uncomment for debugging

        return true;

    }else{
        qDebug()<<"error sending command: "<< msg;
        qDebug() << "Arduino not ready";


        return false;
    }
}

void MainWindow::on_serialInput_returnPressed()
{

}

void MainWindow::on_serialSendButton_clicked()
{
    // send message to arduino
    writeToArduino(ui->serialInput->text());

    // clear joystick field
    ui->serialInput->clear();
}

void MainWindow::onDataAvailable()
{
    // new message received from arduino
    QByteArray bytes;

    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
    bytesReceived.append(bytes);

    bTx = false;
    // only do joystick if all of it has been received.
    // without this the serial port transports line of messages
    // with only 3 or 4 bytes at a time
    if(bytes.contains('\n') || bytes.contains('r'))
    {
        // scroll to bottom of text browser
        ui->serialMonitor->verticalScrollBar()->setValue(ui->serialMonitor->verticalScrollBar()->maximum());

        // move cursor to bottom of text browser
        QTextCursor cursor = ui->serialMonitor->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->serialMonitor->setTextCursor(cursor);

        QString msg = QString::fromLatin1(bytesReceived);
        bytesReceived.clear();

        // if 'u' is first char then update respective display values
        if(msg.at(0) == 'u'){
            if(msg.at(1) == 'v'){
                msg.remove("\n");
                msg.remove("v");
                powerInfoWindow->setVoltageLabel(msg);
                //writeToArduino(msg);

            }else if(msg.at(1) == 'c'){
                msg.remove("\n");
                msg.remove("c");
                powerInfoWindow->setCurrentLabel(msg);
                //writeToArduino(msg);

            }else if(msg.at(1) == 's'){
                msg.remove("\n");
                msg.remove("s");
                powerInfoWindow->setSpeedLabel(msg);
                //writeToArduino(msg);

            }else if(msg.at(1) == 't'){
                msg.remove("\n");
                msg.remove("t");
                powerInfoWindow->setTorqueLabel(msg);
                //writeToArduino(msg);

            }else if(msg.at(1) == 'p'){
                msg.remove("\n");
                msg.remove("p");
                //writeToArduino(msg);

                powerInfoWindow->setPowerLabel(msg);
            }else if(msg.at(1) == 'f'){
                msg.remove("\n");
                msg.remove("f");
                //powerInfoWindow->setForceLabel(msg);
                //writeToArduino(msg);

            }
        }else if(msg.at(0) == '/'){
            msg.remove("/");

            // display serial message
            writeToSerialMonitor(msg);

        }else if(msg == "r"){

            arduinoReady = true;
            emit serialReady();
            writeToSerialMonitor(msg);
        }else{
            writeToSerialMonitor(msg);

        }
    }


}

void MainWindow::on_portMenu_currentIndexChanged(const QString &arg1)
{
    // new serial port selected
    if(port->isOpen()){

        port->close();
        QString portName = arg1;

        if(portName.contains("cu" && "usb")){
            portName.replace(portName.indexOf("cu"),2,"tty");
        }

        qDebug() << "opening " << arg1;
        port->setPortName(portName);
        if(port->open(QextSerialPort::ReadWrite)){

            ui->serialStatusLabel->setText("CONNECTED");
        }else{

            ui->serialStatusLabel->setText("NOT CONNECTED");
            port->close();
        }

    }else{

        qDebug() << "opening " << arg1;
        port->setPortName(arg1);
        if(port->open(QextSerialPort::ReadWrite)){

            ui->serialStatusLabel->setText("CONNECTED");
        }else{

            ui->serialStatusLabel->setText("NOT CONNECTED");
            port->close();
        }

    }

    // TODO reopen serial port with user selection

}

// **********Steering Calibration**********

void MainWindow::on_stopButton_clicked()
{
    if(inCalmode){
        QMessageBox::warning(this,"Throttle Reset","Reset the throttle to the <font color = 'red'> <b> Max Power </b></font> position before continuing.<br> (ie. the position closest to you)");
        inCalmode = false;
        ui->currentModeLabel->setText("MANUAL Mode");
    }
}
void MainWindow::on_leftMotorInButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("CALIBRATION Mode");
    writeToArduino("b/");
}

void MainWindow::on_leftMotorInButton_released()
{
    writeToArduino("S/");
}

void MainWindow::on_leftMotorOutButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("CALIBRATION Mode");
    writeToArduino("a/");
}

void MainWindow::on_leftMotorOutButton_released()
{
    writeToArduino("S/");
}

void MainWindow::on_rightMotorInButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("CALIBRATION Mode");
    writeToArduino("c/");
}

void MainWindow::on_rightMotorInButton_released()
{
    writeToArduino("S/");
}

void MainWindow::on_rightMotorOutButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("CALIBRATION Mode");
    writeToArduino("d/");
}

void MainWindow::on_rightMotorOutButton_released()
{
    writeToArduino("S/");
}

// **********Handle other GUI events**********

void MainWindow::on_autoPilotBtn_clicked()
{
    if(!inCalmode){
        if(!autoPilotOn){
            ui->autoPilotBtn->setText("Autopilot OFF");
            autoPilotOn = true;
            ui->currentModeLabel->setText("<font color='red'>AUTOPILOT </font> Mode");
        }else {
            ui->autoPilotBtn->setText("Autopilot ON");
            autoPilotOn = false;
            ui->currentModeLabel->setText("MANUAL Mode");

        }
        //notify control system to start sending commands to arduino
        kiteController->toggleAutoPilot(autoPilotOn);
    }
}

void MainWindow::on_baudRateMenu_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "9600"){

        port->setBaudRate(BAUD9600);

    }else if( arg1 == "115200"){

        port->setBaudRate(BAUD115200);

    }
}

void MainWindow::on_actionPower_Info_triggered()
{
    powerInfoWindow->show();
}


// **********Reading/Writing to file**********

void MainWindow::Save()
{
    // TODO
    // write data to file
}

void MainWindow::Load()
{
    // TODO
    // read in saved data from file
}

void MainWindow::on_actionSave_triggered()
{
    Save();
}

// **********Handle joystick events**********

void MainWindow::readJoystickState()
{
    // update joystick values and return if disconnected
    if (!joystick.updateState()) return;

    // update values from axes
    float inputx = joystick.getHorizontal();
    float inputy = joystick.getThrottle();
    int currentTurnVal;
    int currentPowerVal;

    // scale value for negative x axis to work with Cyborg 5 Fly joystick
    if(inputx < 0) inputx = inputx ;

    // update current turn/power values
    currentTurnVal = int(inputx*30);
    currentPowerVal = int((inputy+1.0f)*96/2);

    if(!inCalmode && !autoPilotOn){

        // write turn value to Arduino
        if(currentTurnVal != lastTurnVal && currentTurnVal %2 == 0){
            writeToArduino("t" + QString::number(currentTurnVal) + "/");
        }

        // write power value to Arduino
        if(currentPowerVal != lastPowerVal && currentPowerVal %2 == 0){
            writeToArduino("p" + QString::number(currentPowerVal) + "/");
        }
        if(currentTurnVal!=lastTurnVal||currentPowerVal != lastPowerVal){
            dataLogger(QString::number(currentTurnVal),QString::number(currentPowerVal));
}
        // check if 'toggle autopilot' button is pressed
        if(joystick.isKeyPressed(13)){
            if(!inCalmode){
                if(!autoPilotOn){
                    ui->autoPilotBtn->setText("Autopilot OFF");
                    // update current state
                    autoPilotOn = true;
                    ui->currentModeLabel->setText("<font color='red'>AUTOPILOT </font> Mode");
                }else {
                    ui->autoPilotBtn->setText("Autopilot ON");
                    // update current state
                    autoPilotOn = false;
                    ui->currentModeLabel->setText("Manual Mode");
                }
            }
        }
    }

    lastPowerVal = currentPowerVal;
    lastTurnVal = currentTurnVal;

}

void MainWindow::on_actionImage_Processor_triggered()
{
    //imageProcessingWindow->show();
}


void MainWindow::on_forceHandshakeButton_clicked()
{
    arduinoReady = true;
    emit serialReady();
}

void MainWindow::writeToSerialMonitor(QString msg)
{
    // scroll to bottom of text browser
    ui->serialMonitor->verticalScrollBar()->setValue(ui->serialMonitor->verticalScrollBar()->maximum());

    // move cursor to bottom of text browser
    QTextCursor cursor = ui->serialMonitor->textCursor();

    cursor.movePosition(QTextCursor::End);

    ui->serialMonitor->setTextCursor(cursor);

    if(bTx){
        ui->serialMonitor->insertPlainText("Sent: " + msg + "\n");
    }else{
        ui->serialMonitor->insertPlainText("Recieved: " + msg + "\n");

    }

}

bool MainWindow::isSerialReady()
{
    return arduinoReady;
}
void MainWindow::dataLogger(QString msg1,QString msg2)
{
    //save all filtering settings to file

    QString date = QDateTime::currentDateTime().toString();
    date.replace(':','_');

    QString time = QDateTime::currentDateTime().time().toString();
    int timems = QDateTime::currentDateTime().time().msec();

    if(!_dataLoggerFileCreated){
        //find current directory
        QString cPath= QDir::currentPath();
        //create QDir object with current directory
        QDir theDir(cPath);
        //move UP a folder
        bool pathExists = theDir.cdUp();
        if(pathExists)
            theDir.cd("KiteControl-Qt/data");
        else qDebug()<<"error changing path";


        dataLogPath = theDir.path() + "/" + "MotorControlData_" + date +".txt";
    }

    QFile file(dataLogPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        // Do something maybe ?
    }


    QTextStream out(&file);
    // write column titles the first time through
    if(!_dataLoggerFileCreated) {

        qDebug()<<"logging data to: " + dataLogPath +"...";
               out << "TURN" <<","<< "Power"<<"," << "time" << "\n";
        _dataLoggerFileCreated = true;
    }

    // write data to the file
    out <<msg1<<","<<msg2<<","<<time+":"+QString::number(timems) <<"\n";

    file.close();
}

void MainWindow::on_imgProcButton_clicked()
{
    //imageProcessingWindow->show();
}


void MainWindow::on_controlOptions_Button_clicked()
{
    controlWindow->show();
}
