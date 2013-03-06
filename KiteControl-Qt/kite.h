#ifndef KITE_H
#define KITE_H

#include <QObject>
#include <QVector2D>

class Kite : public QObject
{
    Q_OBJECT
public:
    explicit Kite(QObject *parent = 0);

    Kite(int,int,int,int);

    QString name;

    float size;
    //    float x;
    //    float y;

signals:
    
public slots:
    void setName(QString name);

    void setSize(float size);

    void setX(int x);

    void setY(int y);

    void setHeading(int hx,int hy){
        this->_headingX = hx;
        this->_headingY = hy;
    }

    int getX(){return this->_xPos;}
    int getY(){return this->_yPos;}
    QVector2D getHeading(){
        QVector2D temp(_headingX,_headingY);

        return temp;

    }
    QVector2D getKitePos(){


        return QVector2D(this->_xPos,this->_yPos);
    }


private:

    int _xPos;
    int _yPos;

    int _headingX;
    int _headingY;



};

#endif // KITE_H
