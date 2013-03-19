#ifndef QUADRENT_H
#define QUADRENT_H

class Quadrent
{
public:
    Quadrent();

    void setX(int x){
        this->x = x;
    }

    int getX(){
        return x;
    }

private:

    int x;
};

#endif // QUADRENT_H
