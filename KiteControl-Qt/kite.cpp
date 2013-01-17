#include "kite.h"

Kite::Kite(QObject *parent) :
    QObject(parent)
{

}

void Kite::setName(QString name)
{
    this->name = name;
}

void Kite::setSize(float size)
{
    this->size = size;
}

void Kite::setX(float x)
{
    this->x = x;
}

void Kite::setY(float y)
{
    this->y = y;
}
