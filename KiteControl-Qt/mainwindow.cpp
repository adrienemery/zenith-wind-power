#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QScrollBar>
#include <QMessageBox>
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
    colorTracker = new KiteColorTracker(this);
    powerInfoWindow = new PowerInfo(this);
    addKiteWindow = new AddKite(this);
    connect(addKiteWindow,SIGNAL(kiteAdded()),this,SLOT(addKiteToComboBox()));

    connect(this,SIGNAL(destroyed()),colorTracker,SLOT(cleanup()));
    // start Qtimer to poll joystick values every 15ms
    tmr.setInterval(15);
    connect(&tmr,SIGNAL(timeout()),this,SLOT(readJoystickState()));
    tmr.start();

    // load saved data
    Load();

    // initialize stylesheets/values
    ui->panSlider->setValue(90);
    ui->tiltSlider->setValue(90);

    // initialize port info
    port = new QextSerialPort("/dev/tty.usbserial-A6008blW");
    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_2);

    // open port and check if it opened properly
    if(port->open(QextSerialPort::ReadWrite)){
        ui->serialStatusLabel->setText("CONNECTED");
    }else{
        ui->serialStatusLabel->setText("NOT CONNECTED");
        port->close();
    }

    // add ports to port drop down menu
    QList<QextPortInfo> portInfo = QextSerialEnumerator::getPorts();
    for(int i =0; i<portInfo.size(); i++){
        ui->portMenu->addItem(portInfo.value(i).portName);
    }

    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.value(i).portName.contains("usb")){
            ui->portMenu->setCurrentIndex(i);
        }
    }

    ui->baudRateMenu->setCurrentIndex(1);

    // get local ip address
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());

    // display local ip address
    ui->localIP->setText(info.addresses().value(2).toString());

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

    // initialize known kites
    kiteList.push_back(new Kite);
    kiteList.push_back(new Kite);
    kiteList.value(0)->setName("Niash Boxer SLE");
    kiteList.value(0)->setSize(7.5);
    kiteList.value(1)->setName("Liquid Force Havoc");
    kiteList.value(1)->setSize(14.0);
    ui->kiteComboBox->addItem(kiteList.value(0)->name + " " + QString::number(kiteList.value(0)->size)+ "m2");
    ui->kiteComboBox->addItem(kiteList.value(1)->name + " " + QString::number(kiteList.value(1)->size) + "m2");

}

// **********Serial Functions**********

void MainWindow::writeToArduino(QString msg)
{
    if(port->isOpen()){
        QByteArray bytes;
        bytes.append(msg);

        port->write(bytes);
        port->flush();

        //        // scroll to bottom of text browser
        //        ui->serialMonitor->verticalScrollBar()->setValue(ui->serialMonitor->verticalScrollBar()->maximum());

        //        // move cursor to bottom of text browser
        //        QTextCursor cursor = ui->serialMonitor->textCursor();
        //        cursor.movePosition(QTextCursor::End);
        //        ui->serialMonitor->setTextCursor(cursor);

        //        ui->serialMonitor->insertPlainText("Sent: " + msg + "\n");
    }
}

void MainWindow::on_serialInput_returnPressed()
{
    // serial message entered

    writeToArduino(ui->serialInput->text());

    // clear joystick field
    ui->serialInput->clear();
}

void MainWindow::on_serialSendButton_clicked()
{
    // serial message entered

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
            }else if(msg.at(1) == 'c'){
                msg.remove("\n");
                msg.remove("c");
                powerInfoWindow->setCurrentLabel(msg);
            }else if(msg.at(1) == 's'){
                msg.remove("\n");
                msg.remove("s");
                powerInfoWindow->setSpeedLabel(msg);
            }else if(msg.at(1) == 't'){
                msg.remove("\n");
                msg.remove("t");
                powerInfoWindow->setTorqueLabel(msg);
            }else if(msg.at(1) == 'p'){
                msg.remove("\n");
                msg.remove("p");
                powerInfoWindow->setPowerLabel(msg);
            }else if(msg.at(1) == 'f'){
                msg.remove("\n");
                msg.remove("f");
                //powerInfoWindow->setForceLabel(msg);
            }
        }else if(msg.at(0) == '/'){
            msg.remove("/");
            // display serial message
            ui->serialMonitor->insertPlainText(msg);
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

        port->setPortName(portName);
        if(port->open(QextSerialPort::ReadWrite)){
            ui->serialStatusLabel->setText("CONNECTED");
        }else{
            ui->serialStatusLabel->setText("NOT CONNECTED");
            port->close();
        }

    }else{

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

// **********Pan/Tilt Calibration**********

void MainWindow::on_tiltSlider_valueChanged(int value)
{
    writeToArduino("T" + QString::number(value) + "/");
}

void MainWindow::on_panSlider_valueChanged(int value)
{
    writeToArduino("P" + QString::number(value) + "/");
}

// **********Steering Calibration**********

void MainWindow::on_stopButton_clicked()
{
    if(inCalmode){
        QMessageBox::warning(this,"Throttle Reset","Reset the throttle to the <font color = 'red'> <b> Max Power </b></font> position before continuing.<br> (ie. the position closest to you)");
        inCalmode = false;
        ui->currentModeLabel->setText("Manual Mode");
    }
}
void MainWindow::on_leftMotorInButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("Calibration Mode");
    writeToArduino("b/");
}

void MainWindow::on_leftMotorInButton_released()
{
    writeToArduino("S/");
}

void MainWindow::on_leftMotorOutButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("Calibration Mode");
    writeToArduino("a/");
}

void MainWindow::on_leftMotorOutButton_released()
{
    writeToArduino("S/");
}

void MainWindow::on_rightMotorInButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("Calibration Mode");
    writeToArduino("c/");
}

void MainWindow::on_rightMotorInButton_released()
{
    writeToArduino("S/");
}

void MainWindow::on_rightMotorOutButton_pressed()
{
    inCalmode = true;
    ui->currentModeLabel->setText("Calibration Mode");
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
            ui->currentModeLabel->setText("Manual Mode");

        }
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

void MainWindow::on_addKiteButton_clicked()
{
    addKiteWindow->show();
}

void MainWindow::addKiteToComboBox()
{
    ui->kiteComboBox->addItem(addKiteWindow->kiteList.back()->name + " " +
                              QString::number(addKiteWindow->kiteList.back()->size) + "m2");
}

void MainWindow::on_removeKiteButton_clicked()
{
    int answer;
    answer = QMessageBox::question(this,"Remove Kite", "Are you sure?");

    if(answer == QMessageBox::Yes){
        // delete kite from kite list
        addKiteWindow->kiteList.removeAt(ui->kiteComboBox->currentIndex());
        //remove kite from comboBox
        ui->kiteComboBox->removeItem(ui->kiteComboBox->currentIndex());
    }else{
        // Do nothing
    }
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
    if(inputx < 0) inputx = inputx / 0.78;

    // update current turn/power values
    currentTurnVal = int(inputx*20);
    currentPowerVal = int((inputy+1.0f)*96/2);

    if(!inCalmode && !autoPilotOn){

        // write turn value to Arduino
        if(currentTurnVal != lastTurnVal){
            writeToArduino("t" + QString::number(currentTurnVal) + "/");
        }

        // write power value to Arduino
        if(currentPowerVal != lastPowerVal && currentPowerVal %2 == 0){
            writeToArduino("p" + QString::number(currentPowerVal) + "/");
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

void MainWindow::on_pushButton_clicked()
{


}
