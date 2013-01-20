#ifndef POWERINFO_H
#define POWERINFO_H

#include <QDialog>

namespace Ui {
class PowerInfo;
}

class PowerInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit PowerInfo(QWidget *parent = 0);
    ~PowerInfo();

public slots:
    void setVoltageLabel(QString volts);

    void setCurrentLabel(QString amps);

    void setSpeedLabel(QString rpm);

    void setTorqueLabel(QString N_m);

    void setPowerLabel(QString watts);

    void setBattery1(QString percentage);

    void setBattery2(QString percentage);

    float getVoltage();

    float getCurrent();

    float getSpeed();

    float getTorque();

    float getPower();

    int getBatt1();

    int getBatt2();


private:
    Ui::PowerInfo *ui;
};

#endif // POWERINFO_H
