#ifndef TARGETPOINTITEM_H
#define TARGETPOINTITEM_H

#include <QPainter>
#include <QGraphicsItem>

class TargetPointItem : public QGraphicsItem
{
public:
    TargetPointItem();

    TargetPointItem(int x, int y, int width=15, int height=15,int ID=0, bool currentTarget = false);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setWidth(int width);

    void setHeight(int height);

    void setX(qreal x);

    void setY(qreal y);

    void setID(int id);

    void setCurrentTarget(bool val);

    int getID(){return id;}

    bool isCurrentTarget(){return currentTarget;}
    int getX(){return this->x;}
    int getY(){return this->y;}

protected:

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    int x,y;

    int width,height;

    static int s_num;
    int id;

    bool currentTarget;
};

#endif // TARGETPOINTITEM_H
