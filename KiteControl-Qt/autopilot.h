#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include "controlalgorithm.h"

class AutoPilot : public QObject
{
    Q_OBJECT
public:
    explicit AutoPilot(QObject *parent = 0);
    
signals:
    
public slots:

    void enable();

    void dissable();

private:
    bool isOn;

};

#endif // AUTOPILOT_H
