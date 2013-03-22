#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include <QSignalMapper>
#include "powerinfo.h"
#include "addkite.h"
#include <QTimer>
#include "xinputGamepad.h"
#include "imageprocessing.h"
#include "controlalgorithm.h"
#include "controlwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void serialReady();
    
private slots:

    void on_serialInput_returnPressed();

    void onDataAvailable();

    void on_portMenu_currentIndexChanged(const QString &arg1);

    void on_serialSendButton_clicked();

    bool writeToArduino(QString msg);

    void on_stopButton_clicked();

    void on_leftMotorInButton_pressed();

    void on_leftMotorInButton_released();

    void on_leftMotorOutButton_pressed();

    void on_leftMotorOutButton_released();

    void on_rightMotorInButton_pressed();

    void on_rightMotorInButton_released();

    void on_rightMotorOutButton_pressed();

    void on_rightMotorOutButton_released();

    void on_autoPilotBtn_clicked();

    void on_baudRateMenu_currentIndexChanged(const QString &arg1);

    void on_actionPower_Info_triggered();

    void Save();

    void Load();

    void on_actionSave_triggered();

    void readJoystickState();

    void on_actionImage_Processor_triggered();

    void on_forceHandshakeButton_clicked();

    void writeToSerialMonitor(QString msg);

    bool isSerialReady();

    void dataLogger(QString msg1, QString msg2);

    void on_imgProcButton_clicked();


    void on_controlOptions_Button_clicked();

private:
    void setup();

private:
    Ui::MainWindow *ui;

    QextSerialPort *port;

    QByteArray bytesReceived;

    QMenu *selectJoystickMenu;

    QSignalMapper *signalMapper;

    PowerInfo *powerInfoWindow;

    AddKite *addKiteWindow;

    //ImageProcessing *imageProcessingWindow;

    ControlAlgorithm *kiteController;

    ControlWindow *controlWindow;

    QList<Kite*> kiteList;

    XInput joystick;
    QTimer tmr;
    QTimer *handshakeTimer;

    bool f_haveJoystick;

    int lastTurnVal;
    int lastPowerVal;
    bool autoPilotOn;
    bool inCalmode;
    bool arduinoReady;
    bool _dataLoggerFileCreated;

    QString dataLogPath;
};

#endif // MAINWINDOW_H
