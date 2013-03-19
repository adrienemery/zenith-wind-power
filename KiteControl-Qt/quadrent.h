#ifndef QUADRENT_H
#define QUADRENT_H
#include <QString>

class Quadrant
{
public:
    public:
        Quadrant();
        Quadrant(int leftX,int rightX,int topY,int bottomY);
        void setVals(int leftX,int rightX,int topY,int bottomY);
        void setLeftX(int x){this->leftX=x;}
        void setRightX(int x){this->rightX=x;}
        void setTopY(int y){this->topY=y;}
        void setBottomY(int y){this->bottomY=y;}
        int getLeftX(){return this->leftX;}
        int getRightX(){return this->rightX;}
        int getTopY(){return this->topY;}
        int getBottomY(){return this->bottomY;}
    private:

    int leftX,rightX,topY,bottomY;

    QString ID;

    };

#endif // QUADRENT_H
