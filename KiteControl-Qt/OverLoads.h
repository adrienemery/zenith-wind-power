#ifndef OVERLOADS_H
#define OVERLOADS_H

#include "kite.h"
#include <QtCore>

//QDataStream &operator<<(QDataStream &out, const Kite &kite);
//QDataStream &operator>>(QDataStream &in, Kite &kite);

//QDataStream &operator<<(QDataStream &out, const Kite &kite)
//{
//    //Write a kite class to the stream
//    out << kite.name << kite.model << kite.make << kite.year << kite.size;
//    return out;
//}

//QDataStream &operator>>(QDataStream &in, Kite &kite)
//{
//    //Read in a kite class
//    kite = Kite();
//    in >> kite.name >> kite.model >> kite.make >> kite.year >> kite.size;
//    return in;
//}

#endif // OVERLOADS_H
