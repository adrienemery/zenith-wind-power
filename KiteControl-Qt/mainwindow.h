#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include <QSignalMapper>
#include "powerinfo.h"
#include "kitecolortracker.h"
#include "addkite.h"
#include "OverLoads.h"
#include <QTimer>
#include "xinputGamepad.h"
#include "imageprocessing.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_serialInput_returnPressed();

    void onDataAvailable();

    void on_portMenu_currentIndexChanged(const QString &arg1);

    void on_serialSendButton_clicked();

    void writeToArduino(QString msg);

    void on_stopButton_clicked();

    void on_leftMotorInButton_pressed();

    void on_leftMotorInButton_released();

    void on_leftMotorOutButton_pressed();

    void on_leftMotorOutButton_released();

    void on_rightMotorInButton_pressed();

    void on_rightMotorInButton_released();

    void on_rightMotorOutButton_pressed();

    void on_rightMotorOutButton_released();

    void on_panSlider_valueChanged(int value);

    void on_tiltSlider_valueChanged(int value);

    //void joystickSelected(int num);

    //void joystickDisconnected();

    //void axisValueChanged(int axis, int value);

    //void joystickButtonValueChanged(int button, bool value);

    void on_autoPilotBtn_clicked();

    void on_baudRateMenu_currentIndexChanged(const QString &arg1);

    void on_actionPower_Info_triggered();

    void on_addKiteButton_clicked();

    void addKiteToComboBox();

    void on_removeKiteButton_clicked();

    void Save();

    void Load();

    void on_actionSave_triggered();

    void readJoystickState();

    void on_actionImage_Processor_triggered();

private:
    void setup();

private:
    Ui::MainWindow *ui;

    QextSerialPort *port;

    QByteArray bytesReceived;

    //Joystick joystick;

    //QList<QAction *> joysticks;

    QMenu *selectJoystickMenu;

    QSignalMapper *signalMapper;

    PowerInfo *powerInfoWindow;

    AddKite *addKiteWindow;

    ImageProcessing *imageProcessingWindow;

    KiteColorTracker *colorTracker;

    QList<Kite*> kiteList;

    XInput joystick;
    QTimer tmr;

    bool f_haveJoystick;

    int lastTurnVal;
    int lastPowerVal;
    bool autoPilotOn;
    bool inCalmode;
};

#endif // MAINWINDOW_H
