#ifndef QUADRANTITEM_H
#define QUADRANTITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>

class QuadrantItem : public QGraphicsItem
{
public:
    QuadrantItem();

    QuadrantItem(int x, int y,int width,int height,int quadrant);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setWidth(int width);

    void setHeight(int height);

    void setX(qreal x);

    void setY(qreal y);

protected:

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    int x,y;

    int width,height;

    int quadrant;

};

#endif // QUADRANTITEM_H
