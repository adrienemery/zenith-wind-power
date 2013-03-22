#include "quadrantitem.h"

QuadrantItem::QuadrantItem()
{
  x = y = 0;
  setFlag(QGraphicsItem::ItemIsSelectable);

  this->width = 640/2;
  this->height = 480/2;

}

QuadrantItem::QuadrantItem(int x, int y, int width, int height, int quadrant)
{

    setFlag(QGraphicsItem::ItemIsSelectable);

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->quadrant = quadrant;

    if(quadrant == 1){
        x = 0;
        y = -height;
    }else if(quadrant == 2){
        x = -width;
        y = -height;
    }else if(quadrant == 3){
        x = -width;
        y = 0;
    }else if(quadrant == 4){
        x = 0;
        y = 0;
    }else if(quadrant == 5){
        x = -width/2;
        y = -height/2;
    }
}

QRectF QuadrantItem::boundingRect() const
{

    return QRectF(x,y,width,height);
}

void QuadrantItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPen pen(Qt::black);

    pen.setWidth(3);

    if(isSelected()){
        pen.setColor(Qt::red);
    }else{
        pen.setColor(Qt::black);
    }

    painter->setPen(pen);
    painter->drawRect(rect);
}

void QuadrantItem::setWidth(int width)
{
    this->width = width-2;

    if(quadrant == 1){
        x = 0;
        y = -height;
    }else if(quadrant == 2){
        x =  -width;
        y = -height;
    }else if(quadrant == 3){
        x = -width;
        y = 0;
    }else if(quadrant == 4){
        x = 0;
        y = 0;
    }else if(quadrant == 5){
        x = -width/2;
        y = -height/2;
    }

    update();

}

void QuadrantItem::setHeight(int height)
{
    this->height = height-2;

    if(quadrant == 1){
        x = 0;
        y = -height;
    }else if(quadrant == 2){
        x = -width;
        y = -height;
    }else if(quadrant == 3){
        x = -width;
        y = 0;
    }else if(quadrant == 4){
        x = 0;
        y = 0;
    }else if(quadrant == 5){
        x = -width/2;
        y = -height/2;
    }

    update();
}

void QuadrantItem::setX(qreal x)
{
    this->x = x;
}

void QuadrantItem::setY(qreal y)
{
    this->y = y;
}

void QuadrantItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseDoubleClickEvent(event);

}

void QuadrantItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    update();
    QGraphicsItem::mousePressEvent(event);

}


