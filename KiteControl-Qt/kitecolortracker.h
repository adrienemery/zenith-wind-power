#ifndef KITECOLORTRACKER_H
#define KITECOLORTRACKER_H

#include <QObject>
#include <QtCore>
#include "kite.h"

class KiteColorTracker : public QObject
{
    Q_OBJECT
public:
    explicit KiteColorTracker(QObject *parent = 0);

    QTimer *timer;

    Kite *kite;

    int getSampleRate();
    
signals:


    
public slots:
    void update();

    void setSampleRate(int msec);

private:

    int sampleRate;



};

#endif // KITECOLORTRACKER_H
