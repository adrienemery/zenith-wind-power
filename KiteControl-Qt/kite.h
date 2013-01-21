#ifndef KITE_H
#define KITE_H

#include <QObject>

class Kite : public QObject
{
    Q_OBJECT
public:
    explicit Kite(QObject *parent = 0);

    QString name;

    float size;
    float x;
    float y;

signals:
    
public slots:
    void setName(QString name);

    void setSize(float size);

    void setX(float x);

    void setY(float y);

private:


};

#endif // KITE_H
