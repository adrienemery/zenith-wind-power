#include "quadrent.h"

Quadrant::Quadrant()
{

}
Quadrant::Quadrant(int leftX, int rightX, int topY, int bottomY)
{
    this->leftX=leftX;
    this->rightX = rightX;
    this->bottomY=bottomY;
    this->topY = topY;


}

void Quadrant::setVals(int leftX, int rightX, int topY, int bottomY){

    this->leftX=leftX;
    this->rightX = rightX;
    this->bottomY=bottomY;
    this->topY = topY;
}
