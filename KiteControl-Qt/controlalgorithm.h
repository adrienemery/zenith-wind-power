#ifndef CONTROLALGORITHM_H
#define CONTROLALGORITHM_H

#include <QObject>
#include <QtCore>
#include <QVector2D>
#include "imageprocessing.h"
#include "kitecolortracker.h"


class ControlAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit ControlAlgorithm(QObject *parent = 0);
    
signals:
    
public slots:

    void update();

    void setMinX(float x);

    void setMaxX(float x);

    void setMinY(float y);

    void setMaxY(float y);

private:

    QVector2D kitePos;
    QVector2D leftPath;
    QVector2D rightPath;
    QVector2D leftEnd;
    QVector2D leftBegin;
    QVector2D rightEnd;
    QVector2D rightBegin;

    float minX;
    float maxX;
    float minY;
    float maxY;

    KiteColorTracker *kiteColorTracker;
};

#endif // CONTROLALGORITHM_H