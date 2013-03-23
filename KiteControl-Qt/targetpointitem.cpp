#include "targetpointitem.h"

int TargetPointItem::s_num = 0;

TargetPointItem::TargetPointItem()
{
    id = s_num++;
    x = 0;
    y = 0;
    width = 15;
    height = 15;

    setFlag(QGraphicsItem::ItemIsMovable);
}

TargetPointItem::TargetPointItem(int x, int y, int width, int height,bool currentTarget)
{
    id = s_num++;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->currentTarget = currentTarget;


    setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF TargetPointItem::boundingRect() const
{
    return QRectF(x,y,width,height);
}

void TargetPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPen pen(Qt::black);

    pen.setWidth(1);

    painter->setPen(pen);
    if(currentTarget){
        painter->setBrush(QBrush(Qt::green));
    }else{
        painter->setBrush(QBrush(Qt::red));
    }
    painter->drawEllipse(rect);
}

void TargetPointItem::setWidth(int width)
{
   this->width = width;
}


void TargetPointItem::setHeight(int height)
{
    this->height = height;
}

void TargetPointItem::setX(qreal x)
{
    this->x = x;
}

void TargetPointItem::setY(qreal y)
{
    this->y = y;
}

void TargetPointItem::setID(int id)
{
    this->id = id;
}

void TargetPointItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

void TargetPointItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}
