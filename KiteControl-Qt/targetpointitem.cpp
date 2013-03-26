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

TargetPointItem::TargetPointItem(int x, int y, int width, int height, int ID, bool currentTarget)
{
    id = 1;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->currentTarget = currentTarget;
    this->id = ID;


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

    painter->drawText(rect,Qt::AlignCenter,QString::number(id));
}

void TargetPointItem::setWidth(int width)
{
   this->width = width;
   update();
}


void TargetPointItem::setHeight(int height)
{
    this->height = height;
    update();
}

void TargetPointItem::setX(qreal x)
{
    this->x = x;
    update();
}

void TargetPointItem::setY(qreal y)
{
    this->y = y;
    update();
}

void TargetPointItem::setID(int id)
{
    this->id = id;
    update();
}

void TargetPointItem::setCurrentTarget(bool val)
{
   this->currentTarget = val;
    update();
}

void TargetPointItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

void TargetPointItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}
