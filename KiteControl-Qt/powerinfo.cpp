#include "powerinfo.h"
#include "ui_powerinfo.h"
#include <QMovie>

PowerInfo::PowerInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PowerInfo)
{

    ui->setupUi(this);

    QMovie *movie = new QMovie(":/animations/kiteanimationGIF.gif");
    ui->motorDisplayLabel->setMovie(movie);
    movie->start();
}

PowerInfo::~PowerInfo()
{
    delete ui;
}

void PowerInfo::setVoltageLabel(QString volts)
{
    ui->voltageLabel->setText(volts);
}

void PowerInfo::setCurrentLabel(QString amps)
{
    ui->currentLabel->setText(amps);
}

void PowerInfo::setSpeedLabel(QString rpm)
{
    ui->speedLabel->setText(rpm);
}
void PowerInfo::setTorqueLabel(QString N_m)
{
    ui->torqueLabel->setText(N_m);
}

void PowerInfo::setPowerLabel(QString watts)
{
    ui->powerLabel->setText(watts);
}

void PowerInfo::setBattery1(QString percentage)
{
    ui->batt1ProgressBar->setValue(percentage.toInt());
}

void PowerInfo::setBattery2(QString percentage)
{
    ui->batt2progressBar->setValue(percentage.toInt());
}

float PowerInfo::getVoltage()
{
    return ui->voltageLabel->text().toFloat();
}

float PowerInfo::getCurrent()
{
    return ui->currentLabel->text().toFloat();
}

float PowerInfo::getSpeed()
{
    return ui->speedLabel->text().toFloat();

}

float PowerInfo::getTorque()
{
    return ui->torqueLabel->text().toFloat();

}

float PowerInfo::getPower()
{
    return ui->powerLabel->text().toFloat();

}

int PowerInfo::getBatt1()
{
    return ui->batt1ProgressBar->value();

}

int PowerInfo::getBatt2()
{
    return ui->batt2progressBar->value();
}
