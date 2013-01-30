#include "autopilot.h"

AutoPilot::AutoPilot(QObject *parent) :
    QObject(parent)
{

}

void AutoPilot::enable()
{
    isOn = true;
}

void AutoPilot::dissable()
{
    isOn = false;
}



