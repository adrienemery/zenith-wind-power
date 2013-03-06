#include "kite.h"

Kite::Kite(QObject *parent) :
    QObject(parent)
{

}
Kite::Kite(int xPos, int yPos, int headingX, int headingY){

    //create a kite with an xpos, ypos and heading
    _xPos = xPos;
    _yPos = yPos;

    _headingX = headingX;
    _headingY = headingY;


}

void Kite::setName(QString name)
{
    this->name = name;
}

void Kite::setSize(float size)
{
    this->size = size;
}

void Kite::setX(int x)
{
    this->_xPos = x;
}

void Kite::setY(int y)
{
    this->_yPos = y;
}

